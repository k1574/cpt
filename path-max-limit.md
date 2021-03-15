# PATH_MAX limit.

In  case you  rush into  the  problem, that  you have  more slides  than
PATH_MAX, which is  more than four thousand slides, there  is a solution
with catpoint(1):

1. Make a temporary file with the content of your presentation.
2. Create some application to update the temporary file content.
2. Either press reload or use SIGHUP to reload the file and slide.

Using this  solution you lose some  commands in catpoint(1) but  will be
able to scale to infinity.

Another solution  would be  to use some  different kernel  or userspace,
allowing more arguments to a command.

Have fun!

