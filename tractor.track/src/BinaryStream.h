#ifndef _BINARY_STREAM_H_
#define _BINARY_STREAM_H_

#include <RcppEigen.h>

class BinaryStream
{
protected:
    bool swapEndian;
    
    template <typename Type> void swap (Type *value);
    
public:
    void swapEndianness (bool value) { swapEndian = value; }
};

class BinaryInputStream : public BinaryStream
{
private:
    std::ifstream *stream;
    
public:
    BinaryInputStream ()
        : stream(NULL) {}
    BinaryInputStream (std::ifstream *stream)
        : stream(stream) {}
    
    void attach (std::ifstream *stream) { this->stream = stream; }
    void detach () { this->stream = NULL; }
    
    template <typename SourceType> SourceType readValue ();
    template <typename SourceType, typename FinalType> void readVector (std::vector<FinalType> &values, size_t n);
    template <typename SourceType, typename FinalType, int Rows> void readVector (Eigen::Matrix<FinalType,Rows,1> &values, size_t n);
    std::string readString (size_t n);
};

class BinaryOutputStream : public BinaryStream
{
private:
    std::ofstream *stream;
    
public:
    BinaryOutputStream ()
        : stream(NULL) {}
    BinaryOutputStream (std::ofstream *stream)
        : stream(stream) {}
    
    void attach (std::ofstream *stream) { this->stream = stream; }
    void detach () { this->stream = NULL; }
    
    template <typename TargetType> void writeValue (const TargetType value);
    template <typename TargetType> void writeValues (const TargetType value, size_t n);
    template <typename TargetType> void writeArray (TargetType * const pointer, size_t n);
    template <typename TargetType, typename OriginalType> void writeVector (const std::vector<OriginalType> &values, size_t n = 0);
    template <typename TargetType, typename OriginalType, int Rows> void writeVector (const Eigen::Matrix<OriginalType,Rows,1> &values, size_t n = 0);
    void writeString (const std::string &value);
};

#endif
