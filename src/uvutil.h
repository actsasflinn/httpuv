#ifndef UVUTIL_HPP
#define UVUTIL_HPP

#include <string>
#include <uv.h>
#include <Rcpp.h>

inline uv_handle_t* toHandle(uv_timer_t* timer) {
  return (uv_handle_t*)timer;
}
inline uv_handle_t* toHandle(uv_tcp_t* tcp) {
  return (uv_handle_t*)tcp;
}
inline uv_handle_t* toHandle(uv_stream_t* stream) {
  return (uv_handle_t*)stream;
}

inline uv_stream_t* toStream(uv_tcp_t* tcp) {
  return (uv_stream_t*)tcp;
}

void throwLastError(uv_loop_t* pLoop,
  const std::string& prefix = std::string(),
  const std::string& suffix = std::string());

class WriteOp;

class DataSource {
public:
  virtual ~DataSource() {}
  virtual uint64_t length() const = 0;
  virtual uv_buf_t getData(size_t bytesDesired) = 0;
  virtual void freeData(uv_buf_t buffer) = 0;
  virtual void close() {}
};

class ExtendedWrite {
  int _activeWrites;
  uv_stream_t* _pHandle;
  DataSource* _pDataSource;

public:
  ExtendedWrite(uv_stream_t* pHandle, DataSource* pDataSource)
      : _activeWrites(0), _pHandle(pHandle), _pDataSource(pDataSource) {}
  virtual ~ExtendedWrite() {}
  
  virtual void onWriteComplete(int status) = 0;

  void begin();
  friend class WriteOp;

protected:
  void next();

};

#endif // UVUTIL_HPP