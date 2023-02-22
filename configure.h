#ifndef _CONFIGURE_ESP8266_H_
#define _CONFIGURE_ESP8266_H_

#ifdef ESP_DEBUG
//_DEBBUG(" text ", variable 1, " text ", variable 2);
  namespace {
      template<typename T>
      static void _DEBBUG(T last) {
      Serial.println(last);
    }
    
    template<typename T, typename... Args>
    static void _DEBBUG(T head, Args... tail) {
      Serial.print(head);
      Serial.print(' ');
      _DEBBUG(tail...);
    }
  }
#else
    #define _DEBBUG(...)
#endif


#endif
