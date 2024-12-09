const char MODE_1[] = "<       TIME        >"; // Только не бейте за этот костыль
const char MODE_2[] = "<       METEO       >";
const char MODE_3[] = "<     STOPWATCH     >";
const char MODE_4[] = "<       FILES       >";
const char MODE_5[] = "<       VIDEO       >";
const char MODE_6[] = "<       INFO        >";

const char* const MODES[] = {
  MODE_1,
  MODE_2,
  MODE_3,
  MODE_4,
  MODE_5,
  MODE_6
};

const byte MODES_COUNT = sizeof(MODES) / sizeof(MODES[0]);
