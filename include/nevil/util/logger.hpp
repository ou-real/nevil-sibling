#ifndef NEVIL_UTIL_LOGGER_HPP_
#define NEVIL_UTIL_LOGGER_HPP_

#include <iostream>
#include <fstream>

#include "nevil/util/print.hpp"
#include "nevil/util/system.hpp"

namespace nevil
{
  class logger
  {
  public:
    inline logger() {};
    inline virtual ~logger() {};

    inline void start_new_file(const std::string &path, const std::string &file_name)
    {
      if (_output_file.is_open())
          _output_file.close();

      std::string file_path = os::append_path(path, file_name);
      _output_file.open(file_path.c_str(), std::ofstream::out);

      if (!_output_file)
      {
        std::cerr << "Failed to open the file \"" << file_path << "\"\nAborting now..." << std::endl;
        exit(-1);
      }
    }

    inline void flush()
    {
      _output_file.flush();
    }

    inline void close_file()
    {
      _output_file.close();
    }

    template <typename T>
    inline void write(const T &object, bool new_line = false)
    {
      (*this) << object;
      if (new_line)
        (*this) << std::endl;
    }

    template <typename T>
    inline void write(T *arr, size_t size, bool new_line = false)
    {
      (*this) << '[';
      for (size_t i = 0; i < size - 1 ; ++i)
          (*this) << arr[i] << ", ";
      (*this) << arr[size -1] << ']';

      if (new_line)
          (*this) << std::endl;
    }

    template <typename T>
    inline logger &operator<<(const T &object)
    {
      _output_file << object;
      return *this;
    }

    inline logger &operator<<(std::ostream& (*pf) (std::ostream&))
    {
      _output_file << pf;
      return *this;
    }

  protected:
      std::ofstream _output_file;
  };
}
#endif // NEVIL_UTIL_LOGGER_HPP_