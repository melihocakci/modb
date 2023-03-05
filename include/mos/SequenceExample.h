class SequenceExample
{
public:
    SequenceExample();
    void run(bool removeExistingDatabase, const char* fileName);

private:
    // no need for copy and assignment
    SequenceExample(const SequenceExample&);
    void operator = (const SequenceExample&);
};