const char MODE_1[] PROGMEM = "<        TIME       >"; // Толко не бейте за этот костыль
const char MODE_2[] PROGMEM = "<        METEO      >";
const char MODE_3[] PROGMEM = "<        TIMER      >";
const char MODE_4[] PROGMEM = "<      STOPWATCH    >";
const char MODE_5[] PROGMEM = "<        FILES      >";
const char MODE_6[] PROGMEM = "<        INFO       >";

const char* const MODES[] PROGMEM = {
  MODE_1,
  MODE_2,
  MODE_3,
  MODE_4,
  MODE_5,
  MODE_6
};

const byte MODES_COUNT = sizeof(MODES) / sizeof(MODES[0]);
