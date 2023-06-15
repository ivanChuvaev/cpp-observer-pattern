# C++ Observer pattern project

The aim of project was to implement GoF's Observer pattern using c++.

The executable starts two parallel threads that are checking files luigi.txt and mario.txt inside of input_files directory,
whenever file changed the logfile.txt inside logs directory is updated, and changed file gets cleared

there are few commands, you can write them inside either luigi.txt or mario.txt

- exit - stop watch current file
- clear_log - clear logfile.txt
- get_id - get pointer to FileObserver that is watching current file
- draw_pict1 - draws pict1
- draw_pict2 - draws pict2

to stop program you need to write "exit" command in both mario.txt and luigi.txt