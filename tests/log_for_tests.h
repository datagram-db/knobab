//
// Created by giacomo on 25/02/2022.
//

#ifndef KNOBAB_LOG_FOR_TESTS_H
#define KNOBAB_LOG_FOR_TESTS_H

#include <string>

std::string log1 = "/*\n"
                   " * log.txt\n"
                   " * This file is part of knobab\n"
                   " *\n"
                   " * Copyright (C) 2022 - Giacomo Bergami\n"
                   " *\n"
                   " * bpm21 is free software; you can redistribute it and/or modify\n"
                   " * it under the terms of the GNU General Public License as published by\n"
                   " * the Free Software Foundation; either version 2 of the License, or\n"
                   " * (at your option) any later version.\n"
                   " *\n"
                   " * bpm21 is distributed in the hope that it will be useful,\n"
                   " * but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                   " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
                   " * GNU General Public License for more details.\n"
                   " *\n"
                   " * You should have received a copy of the GNU General Public License\n"
                   " * along with bpm21. If not, see <http://www.gnu.org/licenses/>.\n"
                   " */\n"
                   "{ } : A { x= -3.0 };\n"
                   "{ } : B { y= 8.0 };\n"
                   "{ } : C { y= 8.0 };\n"
                   "{ } : A { x= -2.0 } B { y= 7.0 };\n"
                   "{ } : A { x= -2.0 } C { y= 7.0 };\n"
                   "{ } : A { x= 3.0 } B { y= 7.0 };\n"
                   "{ } : A { x= 3.0 } C { y= 7.0 };\n"
                   "{ } : A { x= 4.0 } B { y= 8.0 } B { y= 0.0 }  A { x= 1.0 };\n"
                   "{ } : A { x= 4.0 } C { x= 8.0 } B { y= 0.0 }  A { x= 1.0 };\n"
                   "{ } : A { x= 4.0 } B { y= 0.0 } B { y= 8.0 }  A { x= 1.0 } B { y = -3.0 } B {y = 0.0};\n"
                   "{ } : A { x= 4.0 } B { y= 0.0 }   A { x= 1.0 } B { y = -3.0 } B {y = 0.0};\n"
                   "{ } : A { x= 4.0 } B { y= 0.0 } B { y= 8.0 }  A { x= 1.0 } B { y = -3.0 } ;\n"
                   "{ } : A { x= 4.0 } B { y= 0.0 } A { x= 1.0 } B { y = -3.0 }";

#endif //KNOBAB_LOG_FOR_TESTS_H
