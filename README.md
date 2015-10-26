# freee2jnb-zf

Convert a Freee's Zengin-format to JNB's Zengin-format.
These are somewhat different, though these have a same name.

## Usage
```
freee2jnb-zf [options] filename
```

### options:

#### -o [filename]
output file name (JNB's Zengin-format)

#### -v
verbose (prints records and fields of the input file)

#### -d [MMYY]
update date when output (convert) to the file

#### --help or /?
shows this help message

### filename:
input file name (Freee's Zengin-format)

### example:
```
freee2jnb-zf -v -d 1027 -o jnb.txt freee.txt
```

