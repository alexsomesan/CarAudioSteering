# Host protocol spec

Host communication uses 16 byte transfer packages.

A package has the following structure. Packages shorter than 16 bytes should be padded up to 16.

```
--------------------------------
|CMD_ID 1B|CMD PAYLOAD ...     |
--------------------------------
```

## Available commands
```
---------------------------------
|0x01:1B|RING:1B|DIGI_POT_VAL:2B|
---------------------------------
```
