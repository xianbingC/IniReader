### Features

1. **parse ini configure file**
  
  ```cpp
  #include "inireader.h"
  int main()
  {
       lxr::ini:IniReader ini_reader("./conf.ini"); // parse ini conf
  }
  ```
  
2. **support to get int/float/double/string type value**
  
  ```cpp
       std::string ip = ini_reader["server"]["ip"].toString();
       int port = ini_reader["server"]["port"].toInt();
       float timeout = ini_reader["server"]["timeout"].toFloat();
       double query_time = ini_reader["server"]["query_time"].toDouble();
  ```
  
3. **support to use default value**
  
  ```cpp
       std::string ip = ini_reader["server"]["ip"].toString("0.0.0.0");
       int port = ini_reader["server"]["port"].toInt(5050);
  ```
  
4. **throw exception**
  
  - if configure file doesn't exist, throw exception
    
  - if section field doesn't exist, throw exception
    
  - if key field doesn't exist and doesn't use default value, throw exception
    

### How to import

just include the header file 'inireader.h' in your project, such as follow:

```cpp
#include "inireader.h"

// your code
```
