# Hardware

## Require Hardware
- Arduino
- Color sensor
- Thermal Printer

## Specifications

### Communication
Use color to exchange data.

#### Colors

|Pattern | RGB         | Info                |
|:-------|:------------|:--------------------|
|RED     | (255, 0, 0) | Start or End frame. |
|GREEN   | (0, 255, 0) | Data: (1)           |
|BLUE    | (0, 0, 255) | Data: (0)           |
|GREEN -> BLUE | (SHOW OTHER) | Data: (,)    |
|BLACK   | (0, 0, 0)   | Clock frame.        |
