const char MODE_1[] = "<       TIME        >"; // Только не бейте за этот костыль
const char MODE_2[] = "<       METEO       >";
const char MODE_3[] = "<     STOPWATCH     >";
// const char MODE_4[] = "<       WIFI        >";

const char* const MODES[] = {
  MODE_1,
  MODE_2,
  MODE_3
};

const byte MODES_COUNT = sizeof(MODES) / sizeof(MODES[0]);
