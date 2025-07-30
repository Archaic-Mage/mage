#include "utl/containers/RingBuffer.hpp"

#include <thread>

#include <gtest/gtest.h>

namespace mage::utl::test
{
class RingBufferTest : public ::testing::Test
{
public:
    struct Data
    {
        std::int32_t theIndex;
        std::int32_t theValue;
    };

    void SetUp() override
    {
        theSharedMemoryBuffer = std::make_unique<SharedMemoryBuffer<MaxSize>>();
        theFileMemoryBuffer = std::make_unique<FileMemoryBuffer<MaxSize>>("/tmp/test.buf");
        theRingBuffer = std::make_unique<RingBuffer<Data>>(std::make_unique<SharedMemoryBuffer<MaxSize>>());
    }

    static Data createData()
    {
        return Data{.theIndex = DefaultIndex, .theValue = DefaultValue};
    }

protected:
    static constexpr std::int64_t MaxSize = 8 * 1024 * 1024;
    static constexpr std::int32_t DefaultIndex = 12;
    static constexpr std::int32_t DefaultValue = 10;
    std::unique_ptr<SharedMemoryBuffer<MaxSize>> theSharedMemoryBuffer;
    std::unique_ptr<FileMemoryBuffer<MaxSize>> theFileMemoryBuffer;
    std::unique_ptr<RingBuffer<Data>> theRingBuffer;
};

TEST_F(RingBufferTest, WriteAndReadDataFromSharedBuffer)
{
    std::array<std::int64_t, 5> myData;
    for (std::size_t myI = 0; myI < 5; ++myI)
    {
        myData[myI] = 100 + myI;
    }
    theSharedMemoryBuffer->write(0, std::as_bytes(std::span(myData)));
    std::int64_t myReadNum = 0;
    for (std::size_t myI = 0; myI < 5; ++myI)
    {
        constexpr auto mySizeOfInteger = sizeof(std::int64_t);
        theSharedMemoryBuffer->read(
            myI * mySizeOfInteger,
            std::span(reinterpret_cast<std::byte*>(&myReadNum), mySizeOfInteger));
        EXPECT_EQ(myReadNum, myData[myI]);
    }
}

TEST_F(RingBufferTest, WriteAndReadDataFromFileBuffer)
{
    std::array<std::int64_t, 5> myData;
    for (std::size_t myI = 0; myI < 5; ++myI)
    {
        myData[myI] = 100 + myI;
    }
    theFileMemoryBuffer->write(0, std::as_bytes(std::span(myData)));
    std::int64_t myReadNum = 0;
    for (std::size_t myI = 0; myI < 5; ++myI)
    {
        constexpr auto mySizeOfInteger = sizeof(std::int64_t);
        theFileMemoryBuffer->read(
            myI * mySizeOfInteger,
            std::span(reinterpret_cast<std::byte*>(&myReadNum), mySizeOfInteger));
        EXPECT_EQ(myReadNum, myData[myI]);
    }
}

TEST_F(RingBufferTest, SingleThreadedReadAndWrite)
{
    std::array<Data, 5> myData;
    for (std::size_t myI = 0; myI < 5; ++myI)
    {
        myData.at(myI) = createData();
    }

    auto myProducer = RingBuffer<Data>::Producer(*theRingBuffer);
    auto myConsumer = RingBuffer<Data>::Consumer(*theRingBuffer);

    EXPECT_TRUE(myProducer.write(std::span(myData)));

    std::array<Data, 5> myReadBuffer;
    EXPECT_TRUE(myConsumer.read(std::span(myReadBuffer)));
    for (std::size_t myI = 0; myI < 5; ++myI)
    {
        EXPECT_EQ(myReadBuffer.at(myI).theIndex, myData.at(myI).theIndex);
        EXPECT_EQ(myReadBuffer.at(myI).theValue, myData.at(myI).theValue);
    }

    EXPECT_FALSE(myConsumer.read(std::span(myReadBuffer)));
}

TEST_F(RingBufferTest, MultiThreadedReadAndWrite)
{
    std::array<Data, 1024> myData;
    std::ranges::fill_n(myData.begin(), myData.size(), createData());

    std::thread myProducerThread(
        [this, &myData]()
        {
            auto myProducer = RingBuffer<Data>::Producer(*theRingBuffer);
            while (myProducer.write(std::span(myData.begin(), 8)))
            {
                std::this_thread::yield();
            }
        });

    std::vector<Data> myReadData;

    std::thread myConsumerThread(
        [this, &myData, &myReadData]()
        {
            auto myConsumer = RingBuffer<Data>::Consumer(*theRingBuffer);
            std::array<Data, 4> myReadBuffer;
            while (myReadData.size() < myData.size())
            {
                if (myConsumer.read(std::span(myReadBuffer)))
                {
                    myReadData.append_range(myReadBuffer);
                }
                else
                {
                    std::this_thread::yield();
                }
            }
        });

    myProducerThread.join();
    myConsumerThread.join();

    for (std::size_t myI = 0; myI < myData.size(); ++myI)
    {
        EXPECT_EQ(myReadData.at(myI).theIndex, myData.at(myI).theIndex);
        EXPECT_EQ(myReadData.at(myI).theValue, myData.at(myI).theValue);
    }
}

} // namespace mage::utl::test
