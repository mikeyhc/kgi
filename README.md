kgi
===

Simple CGI Framework

TODO
----
* make `kgi_add_data` use vargs, concat into single string
* when more work is done on kstring (namely ksprintf) switch over more of
  the string functions

Known Bugs
----------
* the arraylist implementations mixes both signed and unsigned longs, any
  stupidly large arraylists will find the highest elements unavailable, the
  only fix I can think of it for it is checking for overflow in the set 
  method and preventing adds at that size
