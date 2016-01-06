
// Standard library:
#include <iostream>

// Third party :
// - Bayeux/datatools:
#include <datatools/exception.h>
#include <datatools/logger.h>

// This project:
#include "memory.h"
#include "build_mem1.h"

int main(void)
{
  DT_LOG_NOTICE(datatools::logger::PRIO_ALWAYS, "Hello, SuperNEMO!");

  snemo::trigger::mem1_type mem1;
  snemo::trigger::mem1_builder m1b;
  m1b.build(mem1);
  mem1.store_to_file("mem1.conf");

  DT_LOG_NOTICE(datatools::logger::PRIO_ALWAYS, "Bye bye!");
  return 0;
}
