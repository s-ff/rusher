# Rusher 
A lightweight brute-forcing multi-threaded CLI to scan websites written in C.

## Motivation
### Why write this tool? 
Why not?

I wrote this tool because I wanted a faster bruter in C. Also, tweak it whenever I want.

##  Build Status
[![Build Status](https://travis-ci.com/soufianefariss/rusher.svg?branch=mmap)](https://travis-ci.com/soufianefariss/rusher)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Usage
```
Usage: rusher [-qrv?V] [-a USER_AGENT] [-o FILE] [-s CODES] [-t THREADS]
            [-w WORDLIST] [--useragent=USER_AGENT] [--output=FILE] [--quiet]
            [--followredirects] [--statuscodes=CODES] [--threads=THREADS]
            [--verbose] [--wordlist=WORDLIST] [--help] [--usage] [--version]
            URL
```

## Command line options
_Rusher_ doesn't provide many options. You can list them by running `rusher --help`:
```
Usage: rusher [OPTION...] URL
Rusher - brute force directories and file names on web applications.

  -a, --useragent=USER_AGENT Set the User-Agent parameter
  -o, --output=FILE          Output to FILE instead of standard output
  -q, --quiet                Don't produce any output
  -r, --followredirects      Follow redirects (default no)
  -s, --statuscodes=CODES    Status codes to scan for (default
                             200,204,301,302,307,401,403)
  -t, --threads=THREADS      Number of concurrent threads (default 10)
  -v, --verbose              Produce verbose output
  -w, --wordlist=WORDLIST    Path to wordlist
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.

Report bugs to <soufiane.fariss@um5s.net.ma>.
```


## Installation
Currently, only GNU/Linux is supported. 
###### From source:
Clone the project:
```
$ git clone https://github.com/soufianefariss/rusher
$ cd rusher
```

Then, make the program:
```
$ make
```
## License
MIT © [S. Fariss](github.com/soufianefariss)
