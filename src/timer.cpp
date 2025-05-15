# include "timer.h"

/// @brief Check if delay is reached
/// @param lastMillis Last millis values for ref 
/// @param delay in ms
/// @return 
bool isTimeout(uint32_t* lastMillis, uint32_t delay)
{
  uint32_t _currentMillis;  
  return isTimeout(lastMillis, &_currentMillis, delay, true);
}

/// @brief Check if delay is reached
/// @param lastMillis Last millis values for ref
/// @param _currentMillis Return currentMillis
/// @param delay in ms
/// @param reset reset lastMillis to _currentMillis if delay is reached
/// @return 
bool isTimeout(uint32_t* lastMillis, uint32_t* _currentMillis, uint32_t delay, bool reset)
{
    *_currentMillis = millis();
    
    // Avoid overflow, but introduce inaccuracy
    if ( *lastMillis > *_currentMillis ){
        *lastMillis = *_currentMillis;
    }

    if(*_currentMillis - *lastMillis >= delay)
    {
      if (reset)
      {
        *lastMillis = *_currentMillis;
      }
      return true;
    }
    return false;
}