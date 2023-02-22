//
// Created by guangjinhuo on 2019-02-14.
//

#ifndef TLVREADER_H
#define TLVREADER_H

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include "Config.h"

namespace cm {
    namespace tlv {

        template<class TTag>
        class TLVReader {
        public:
            TLVReader() {
                _offset = 0;
                _capacity = 0;
            }

            TLVReader(
                    const char *data,
                    const size_t length,
                    const size_t offset = 0
            ) : _data(data), _offset(offset), _capacity(length) {
            }

//            TLVReader(const TLVReader &rhs) {
//                _data = rhs._data;
//                _offset = rhs._offset;
//                _capacity = rhs._capacity;
//            }
//
            ~TLVReader() {}

            bool eof() const { return _offset >= _capacity; }

            size_t size() const { return _offset; }
            size_t capacity() const { return _capacity; }
            const char *data() const { return _data; }

            void reset() {
                _offset = 0;
            }

            void reset(const char *data, const size_t length) {
                _data = data;
                _offset = 0;
                _capacity = length;
            }

            TTag getTag() {
                if (_checkOverflow(sizeof(TTag))) {
                    TTag tag = *reinterpret_cast<TTag *>((unsigned char *) _data + _offset);
                    _offset += sizeof(TTag);
                    return tag;
                }
                return static_cast<TTag>(-1);
            }

            void jumpValue() {
                if (_checkOverflow(sizeof(uint32_t))) {             // NOLINT
                    uint32_t totalSize = *reinterpret_cast<uint32_t *>(const_cast<char *>(_data) + _offset);
                    _offset += sizeof(uint32_t) + totalSize;        // NOLINT
                }
            }

            void jumpValue(size_t &totalSize) {
                if (_checkOverflow(sizeof(uint32_t))) {             // NOLINT
                    totalSize = *reinterpret_cast<uint32_t *>(const_cast<char *>(_data) + _offset);
                    _offset += sizeof(uint32_t) + totalSize;        // NOLINT
                }
            }

            bool getBool(bool &value) { return _getValue(value); }
            bool getUint8(uint8_t &value) { return _getValue(value); }
            bool getInt16(int16_t &value) { return _getValue(value); }
            bool getInt32(int32_t &value) { return _getValue(value); }
            bool getInt64(int64_t &value) { return _getValue(value); }
            bool getFloat32(float &value) { return _getValue(value); }
            bool getFloat64(double &value) { return _getValue(value); }

            bool getString(std::string &value) { return _getStringValue(value); }
            bool getBoolArray(std::vector<bool> &value) { return _getValueArray(value); }
            
            bool getInt64Array(std::vector<int64_t> &value) { return _getValueArray(value); }
            bool getFloat64Array(std::vector<double> &value) { return _getValueArray(value); }
            bool getStringArray(std::vector<std::string> &value) { return _getStringValueArray(value); }

            bool getUint8Array(int *size, uint8_t **value) { return _getValueArray(size, value); }
            bool getInt16Array(int *size, int16_t **value) { return _getValueArray(size, value); }
            bool getInt32Array(int *size, int32_t **value) { return _getValueArray(size, value); }
            bool getFloat32Array(int *size, float **value) { return _getValueArray(size, value); }

            bool getBuffer(std::unique_ptr<char[]> buffer, size_t &size) {
                if (_checkOverflow(sizeof(uint32_t))) {         // NOLINT
                    size = *reinterpret_cast<const uint32_t *>(_data + _offset);
                    _offset += sizeof(uint32_t);                // NOLINT
                    if (_checkOverflow(size)) {
                        char *buf = new char[size];
                        memcpy(buf, _data + _offset, size);
                        _offset += size;
                        buffer.reset(buf);
                        return true;
                    }
                }
                return false;
            }

            bool getTlv(TLVReader<TTag> &tlv) {
                if (_checkOverflow(sizeof(uint32_t))) {         // NOLINT
                    uint32_t totalSize = *reinterpret_cast<uint32_t *>(const_cast<char *>(_data) + _offset);
                    _offset += sizeof(uint32_t);                // NOLINT
                    if (_checkOverflow(totalSize)) {
                        tlv.reset(_data + _offset, totalSize);
                        _offset += totalSize;
                        return true;
                    }
                }
                return false;
            }

            bool getTlvArray(std::vector<TLVReader<TTag> > &tlvs) {
                if (_checkOverflow(sizeof(uint32_t))) {         // NOLINT
                    uint32_t totalSize = *reinterpret_cast<uint32_t *>(const_cast<char *>(_data) + _offset);
                    _offset += sizeof(uint32_t);                // NOLINT
                    if (_checkOverflow(totalSize)) {
                        while (totalSize > 0) {
                            size_t size = 0;
                            const char *buf = nullptr;

                            _getLVBuffer(&buf, size);
                            totalSize -= sizeof(uint32_t) + size;       // NOLINT

                            TLVReader<TTag> reader(buf, static_cast<size_t>(size));
                            tlvs.push_back(reader);
                        }
                        return true;
                    }
                }
                return false;
            }

        private:

            bool _checkOverflow(int bytes) {
                if (_offset + bytes <= _capacity) {
                    return true;
                }
#ifdef assert
                assert(0);
#endif//assert
                return false;
            }

            template<class TValue>
            bool _getValue(TValue &value) {
                if (_checkOverflow(sizeof(uint32_t) + sizeof(TValue))) {        // NOLINT
                    uint32_t totalSize = *reinterpret_cast<uint32_t *>(const_cast<char *>(_data) + _offset);
#ifdef assert
                    assert(totalSize == sizeof(TValue));
#endif//assert
                    _offset += sizeof(uint32_t);                // NOLINT

                    memcpy(&value, _data + _offset, totalSize);
                    // value = *reinterpret_cast<TValue *>(const_cast<char *>(_data) + _offset);

                    _offset += sizeof(TValue);
                    return true;
                }
                return false;
            }
            
            template<class TValue>
            bool _getValueArray(int *size, TValue **value) {
                if (_checkOverflow(sizeof(uint32_t))) {                 // NOLINT
                    uint32_t valueLength = *reinterpret_cast<uint32_t *>(const_cast<char *>(_data) + _offset);
                    _offset += sizeof(uint32_t);                // NOLINT

                    if ((valueLength & (sizeof(TValue) - 1)) == 0 && _checkOverflow(valueLength)) {
                        *size = valueLength / sizeof(TValue);

#ifdef BK_PLATFORM_IOS
                        *value = new TValue[*size];
                        memcpy(*value, _data + _offset, valueLength);
#else
                        *value = reinterpret_cast<TValue *>(const_cast<char *>(_data) + _offset);
#endif

                        _offset += valueLength;
                        return true;
                    }
                }

                *size = 0;
                *value = nullptr;
                return false;
            }

            template<class TValue>
            bool _getValueArray(std::vector<TValue> &arr) {
                if (_checkOverflow(sizeof(uint32_t))) {         // NOLINT
                    uint32_t totalSize = *reinterpret_cast<uint32_t *>(const_cast<char *>(_data) + _offset);
#ifdef assert
                    assert(totalSize % sizeof(TValue) == 0);
#endif//assert
                    _offset += sizeof(uint32_t);                // NOLINT

                    if (_checkOverflow(totalSize)) {
                        unsigned int totalCount = totalSize / sizeof(TValue);

                        arr.resize(totalCount);
                        memcpy(&arr[0], _data + _offset, totalSize);
                        _offset += totalSize;
                        //for (int i = 0; i < totalCount; ++i) {
                        //    arr.push_back(*reinterpret_cast<TValue *>(const_cast<char *>(_data) + _offset));
                        //    _offset += sizeof(TValue);
                        //}

                        return true;
                    }
                }
                return false;
            }

            bool _getStringValue(std::string &value) {
                if (_checkOverflow(sizeof(uint32_t))) {             // NOLINT
                    uint32_t totalSize = *reinterpret_cast<uint32_t *>(const_cast<char *>(_data) + _offset);
#ifdef assert
                    assert(totalSize < 1024 * 1024 * 16); // 16M string??
#endif//assert
                    _offset += sizeof(uint32_t);                    // NOLINT

                    value.assign(_data + _offset, totalSize);
                    _offset += totalSize;

                    return true;
                }
                return false;
            }

            bool _getStringValueArray(std::vector<std::string> &arr) {
                if (_checkOverflow(sizeof(uint32_t))) {             // NOLINT
                    uint32_t totalSize = *reinterpret_cast<uint32_t *>(const_cast<char *>(_data) + _offset);
                    _offset += sizeof(uint32_t);                    // NOLINT

                    if (_checkOverflow(totalSize)) {
                        while (totalSize > 0) {
                            size_t size = 0;
                            const char *buf = nullptr;

                            _getLVBuffer(&buf, size);
                            totalSize -= sizeof(uint32_t) + size;       // NOLINT

                            arr.push_back(std::string(buf, size));
                        }

                        return true;
                    }
                }
                return false;
            }

            bool _getLVBuffer(const char **buf, size_t &len) {
                len = *reinterpret_cast<uint32_t *>(const_cast<char *>(_data) + _offset);
                _offset += sizeof(uint32_t);            // NOLINT

                *buf = const_cast<char *>(_data) + _offset;
                _offset += len;
                return false;
            }

            const char *_data;
            size_t _offset;
            size_t _capacity;
        };

        template<class TTag>
        uint32_t getByteSizeWithTag(TLVReader<TTag> &tlvReader, TTag tag) {
            uint32_t totalSize = 0;
            while (!tlvReader.eof()) {
                uint8_t _tag = tlvReader.getTag();
                if (_tag == tag) {
                    tlvReader.jumpValue(reinterpret_cast<size_t &>(totalSize));
                    break;
                } else {
                    tlvReader.jumpValue();
                }
            }
            tlvReader.reset();
            return totalSize;
        }
    }
}

typedef cm::tlv::TLVReader<uint8_t> CMTLVReader;

#endif
