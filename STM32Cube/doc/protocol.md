# Host protocol spec

Host communication uses 16 byte transfer packages.

A package has the following structure. Packages shorter than 16 bytes should be padded up to 16.

```
--------------------------------
| CMD_ID 1B | CMD PAYLOAD ...  |
--------------------------------
```

## Available commands

**Set potentiometer value**
```
---------------------------------------
| 0x01:1B | RING:1B | DIGI_POT_VAL:2B |
---------------------------------------
```

**Report ADC value**

From host:
```
----------------------------------
| 0x02:1B | ENABLE:1B | (N/A):2B |
----------------------------------
```

To host:
```
-----------------------------------
| 0x02:1B | ADC_VAL:2B | (N/A):1B |
-----------------------------------
```

