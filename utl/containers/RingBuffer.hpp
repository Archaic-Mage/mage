#pragma once

#include <array>
#include <atomic>
#include <cstdint>
#include <fstream>
#include <span>
#include <vector>

namespace mage::utl
{

class StorageInterface
{
public:
    virtual ~StorageInterface() = default;
    virtual std::size_t getStorageCapacity() = 0;
    virtual void write(std::size_t anIndex, std::span<const std::byte> aData) = 0;
    virtual void read(std::size_t anIndex, std::span<std::byte> aOutBuffer) = 0;
};

template <std::size_t SizeV>
class SharedMemoryBuffer final : public StorageInterface
{
public:
    SharedMemoryBuffer() = default;

    std::size_t getStorageCapacity() override
    {
        return SizeV;
    }

    void write(const std::size_t anIndex, const std::span<const std::byte> aData) override
    {
        if (anIndex + aData.size() > SizeV)
        {
            throw std::out_of_range("SharedMemoryBuffer::write");
        }
        std::memcpy(theBuffer.data() + anIndex, aData.data(), aData.size());
    }

    void read(const std::size_t anIndex, std::span<std::byte> aOutBuffer) override
    {
        if (anIndex + aOutBuffer.size() > SizeV)
        {
            throw std::out_of_range("SharedMemoryBuffer::read");
        }
        std::memcpy(aOutBuffer.data(), theBuffer.data() + anIndex, aOutBuffer.size());
    }

private:
    std::array<std::byte, SizeV> theBuffer;
};

template <std::size_t SizeV>
class FileMemoryBuffer final : public StorageInterface
{
public:
    explicit FileMemoryBuffer(const std::string& aFileName) :
            theFile(aFileName, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc)
    {
        if (!theFile)
        {
            throw std::runtime_error("Failed to open file");
        }
        // Allocate file size
        theFile.seekp(SizeV);
        constexpr char myZero = 0;
        theFile.write(&myZero, 1);
        theFile.flush();
    }

    std::size_t getStorageCapacity() override
    {
        return SizeV;
    }

    void write(const std::size_t anIndex, const std::span<const std::byte> aData) override
    {
        if (anIndex + aData.size() > SizeV)
        {
            throw std::out_of_range("FileMemoryBuffer::write");
        }
        theFile.seekp(anIndex);
        theFile.write(reinterpret_cast<const char*>(aData.data()), aData.size());
        theFile.flush();
    }

    void read(const std::size_t anIndex, std::span<std::byte> aOutBuffer) override
    {
        if (anIndex + aOutBuffer.size() > SizeV)
        {
            throw std::out_of_range("FileMemoryBuffer::read");
        }
        theFile.seekg(anIndex);
        theFile.read(reinterpret_cast<char*>(aOutBuffer.data()), aOutBuffer.size());
    }

private:
    std::fstream theFile;
};

template <typename DataTypeT>
class RingBuffer
{
public:
    explicit RingBuffer(std::unique_ptr<StorageInterface> anStorage) :
            theCapacity(anStorage->getStorageCapacity() / sizeof(DataTypeT)),
            theBufferStorage(std::move(anStorage))
    {
    }

    class Producer
    {
    public:
        explicit Producer(RingBuffer& aBuffer) : theBuffer(aBuffer) {}

        bool write(const std::span<const DataTypeT> aData)
        {
            return theBuffer.pushImpl(aData);
        }

    private:
        RingBuffer& theBuffer;
    };

    class Consumer
    {
    public:
        explicit Consumer(RingBuffer& aBuffer) : theBuffer(aBuffer) {}

        bool read(std::span<DataTypeT> aOutputData)
        {
            return theBuffer.pullImpl(aOutputData);
        }

    private:
        RingBuffer& theBuffer;
    };

private:
    bool pushImpl(const std::span<const DataTypeT> aWriteableData)
    {
        const auto myCurrPushIndex = thePushIndex.load(std::memory_order_relaxed);
        const auto myWriteSize = aWriteableData.size();
        const auto myNextPushIndex = (myCurrPushIndex + myWriteSize) % theCapacity;
        if (myNextPushIndex == thePullIndex.load(std::memory_order_acquire))
        {
            return false;
        }
        theBufferStorage->write(myCurrPushIndex * sizeof(DataTypeT), std::as_bytes(aWriteableData));
        thePushIndex.store(myNextPushIndex, std::memory_order_release);
        return true;
    }

    bool pullImpl(std::span<DataTypeT> aOutputData)
    {
        const auto myCurrPullIndex = thePullIndex.load(std::memory_order_relaxed);
        const auto myReadSize = aOutputData.size();
        const auto myNextPullIndex = (myCurrPullIndex + myReadSize) % theCapacity;
        if (myNextPullIndex > thePushIndex.load(std::memory_order_acquire))
        {
            return false;
        }
        theBufferStorage->read(myCurrPullIndex * sizeof(DataTypeT), std::as_writable_bytes(aOutputData));
        thePullIndex.store(myNextPullIndex, std::memory_order_release);
        return true;
    }

    std::size_t theCapacity;
    std::unique_ptr<StorageInterface> theBufferStorage;
    std::atomic<std::int32_t> thePushIndex; // points to index to push new data
    std::atomic<std::int32_t> thePullIndex; // points to index to pull data
};
} // namespace mage::utl
