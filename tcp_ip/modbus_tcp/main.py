import time
from pyModbusTCP.client import ModbusClient

REG_COIL_START      = 0
REG_COIL_NREGS 		= 16

REG_DESCRETES_START	= 0
REG_DESCRETES_NREGS	= 16

REG_INPUT_START 	= 0
REG_INPUT_NREGS 	= 8

REG_HOLDING_START 	= 0
REG_HOLDING_NREGS 	= 8

client = ModbusClient(host="192.168.7.99", port=502, unit_id=1, auto_open=True)

while True:
    try:
        print('-----------------------')
        # ------------------ read coils ------------------
        raw = client.read_coils(REG_COIL_START, REG_COIL_NREGS)
        # if success display registers
        if raw:
            coils_l = [int(s) for s in raw]
            print('coil         0-16:   %s' % coils_l)
        else:
            print('unable to read coils')
        time.sleep(0.5)
        # ------------------ read descrete ------------------
        raw = client.read_discrete_inputs(REG_DESCRETES_START, REG_DESCRETES_NREGS)
        # if success display registers
        if raw:
            descretes = [int(s) for s in raw]
            print('descrete     0-16:   %s' % descretes)
        else:
            print('unable to read descretes')
        time.sleep(0.5)
        # ------------------ read input reg  ------------------
        input_reg = client.read_input_registers(REG_INPUT_START, REG_INPUT_NREGS)
        # if success display registers
        if input_reg:
            print('input reg    0-8:    %s' % input_reg)
        else:
            print('unable to read descretes')
        time.sleep(0.5)
        # ------------------ read holding ------------------
        hold_reg = client.read_holding_registers(REG_HOLDING_START, REG_HOLDING_NREGS)
        # if success display registers
        if hold_reg:
            print('holding reg  0-8:    %s' % hold_reg)
        else:
            print('unable to read descretes')
        time.sleep(0.5)
    except KeyboardInterrupt as err:
        print("Error ".format(err))
        break
        