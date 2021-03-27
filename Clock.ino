//****************************************************************************\\
//**** Clase para convertir TIME en segundos 

class TimeSpan {
public:
  TimeSpan (int32_t seconds = 0);
  TimeSpan (int16_t days, int8_t hours, int8_t minutes, int8_t seconds);
  TimeSpan (const TimeSpan& copy);

  /*!
      @brief  Number of days in the TimeSpan
              e.g. 4
      @return int16_t days
  */
  int16_t days() const         { return _seconds / 86400L; }
  /*!
      @brief  Number of hours in the TimeSpan
              This is not the total hours, it includes the days
              e.g. 4 days, 3 hours - NOT 99 hours
      @return int8_t hours
  */
  int8_t  hours() const        { return _seconds / 3600 % 24; }
  /*!
      @brief  Number of minutes in the TimeSpan
              This is not the total minutes, it includes days/hours
              e.g. 4 days, 3 hours, 27 minutes
      @return int8_t minutes
  */
  int8_t  minutes() const      { return _seconds / 60 % 60; }
  /*!
      @brief  Number of seconds in the TimeSpan
              This is not the total seconds, it includes the days/hours/minutes
              e.g. 4 days, 3 hours, 27 minutes, 7 seconds
      @return int8_t seconds
  */
  int8_t  seconds() const      { return _seconds % 60; }
  /*!
      @brief  Total number of seconds in the TimeSpan, e.g. 358027
      @return int32_t seconds
  */
  int32_t totalseconds() const { return _seconds; }

  TimeSpan operator+(const TimeSpan& right);
  TimeSpan operator-(const TimeSpan& right);

protected:
  int32_t _seconds;   ///< Actual TimeSpan value is stored as seconds
};
/**************************************************************************/
/*!
    @brief  Create a new TimeSpan object in seconds
    @param seconds Number of seconds
*/
/**************************************************************************/
TimeSpan::TimeSpan (int32_t seconds):
  _seconds(seconds)
{}

/**************************************************************************/
/*!
    @brief  Create a new TimeSpan object using a number of days/hours/minutes/seconds
            e.g. Make a TimeSpan of 3 hours and 45 minutes: new TimeSpan(0, 3, 45, 0);
    @param days Number of days
    @param hours Number of hours
    @param minutes Number of minutes
    @param seconds Number of seconds
*/
/**************************************************************************/
TimeSpan::TimeSpan (int16_t days, int8_t hours, int8_t minutes, int8_t seconds):
  _seconds((int32_t)days*86400L + (int32_t)hours*3600 + (int32_t)minutes*60 + seconds)
{}

/**************************************************************************/
/*!
    @brief  Copy constructor, make a new TimeSpan using an existing one
    @param copy The TimeSpan to copy
*/
/**************************************************************************/
TimeSpan::TimeSpan (const TimeSpan& copy):
  _seconds(copy._seconds)
{}

/**************************************************************************/
/*!
    @brief  Add two TimeSpans
    @param right TimeSpan to add
    @return New TimeSpan object, sum of left and right
*/
/**************************************************************************/
TimeSpan TimeSpan::operator+(const TimeSpan& right) {
  return TimeSpan(_seconds+right._seconds);
}

/**************************************************************************/
/*!
    @brief  Subtract a TimeSpan
    @param right TimeSpan to subtract
    @return New TimeSpan object, right subtracted from left
*/
/**************************************************************************/
TimeSpan TimeSpan::operator-(const TimeSpan& right) {
  return TimeSpan(_seconds-right._seconds);
}
