# BinaryProtocolClassModule

# class declaration

# define the special characters
CH_SOH = 0x01
CH_STX = 0x02
CH_EOT = 0x04
CH_ACK = 0x06
CH_DLE = 0x10
CH_NAK = 0x15
CH_SPC = 0xEF

#define the receive states
RCV_STATE_IDLE = 0
RCV_STATE_SRCA = 1
RCV_STATE_DSTA = 2
RCV_STATE_CMND = 3
RCV_STATE_OPT1 = 4
RCV_STATE_OPT2 = 5
RCV_STATE_SEQN = 6
RCV_STATE_CTRL = 7
RCV_STATE_DATA = 8
RCV_STATE_CHEK = 9

class BinaryProtocol():
    """
    binary protocol handler

    """
    # initialization
    def __init__(self, multimode, seqenable):
        self.multimode = multimode
        self.seqenable = seqenable
        self.checksum = 0
        self.rcvstate = RCV_STATE_IDLE
        self.dstaddr = 0
        self.srcaddr = 0
        self.command = 0
        self.option1 = 0
        self.option2 = 0
        self.rcvsequence = 0
        self.xmtsequence = 1
        self.buffer = [0]
        self.status = 0

    # get a copy of the message buffer
    def GetMessage(self):
        # return a copy
        return (self.buffer.copy())
    
    # get the returned command
    def GetCommand(self):
        return(self.command)
    
    # get the returned option1
    def GetOption1(self):
        return(self.option1)
    
    #get the returned option2
    def GetOption2(self):
        return(self.option2)

    # begin message
    def SendMessage(self, command, option1, option2, dstadr, srcadr, message):
        # clear buffer/checksum
        self.buffer.clear()
        self.checksum = 0

        # stuff header
        self.StuffXmit(CH_DLE, False)
        self.StuffXmit(CH_SOH, False)

        # check for multi mode
        if (self.multimode):
            # stuff the destination/source address
            self.StuffXmit(dstadr, False)
            self.StuffXmit(srcadr, False)

        # stuff command/option
        self.StuffXmit(command, True)
        self.StuffXmit(option1, True)
        self.StuffXmit(option2, True)

        # check for sequence
        if (self.seqenable):
            # stuff the transmit sequence
            self.StuffXmit(self.xmtsequence, False)
            self.xmtsequence += 1

        # checkf for data
        if (len(message) != 0):
            # stuff the data header
            self.StuffXmit(CH_DLE, False)
            self.StuffXmit(CH_STX, False)

            # now stuff the data
            for data in message:
                self.StuffXmit(data, True)

        # stuff the trailer/checksum
        self.StuffXmit(CH_DLE, False)
        self.StuffXmit(CH_EOT, False)

        # twos complement the checksum
        self.checksum = ~self.checksum
        self.checksum += 1
        self.checksum &= 0xFF
        self.StuffXmit(self.checksum, False)

        # return a copy
        return (self.buffer.copy())

    # stuff byte in transmit buffer
    def StuffXmit(self, data, escflag):
        # stuff the data/increment the index/add it to the checksum
        self.buffer.append(data)
        self.checksum += data

        # if data stuffing is enabled and the data is DLE
        if ((escflag) and (data == CH_DLE)):
            # call ourselfs again to stuff the special character
            self.StuffXmit(CH_SPC, False)

    # process a receive character
    def ProcessRcv(self, data):
        # add the checkum
        self.checksum += data
        self.checksum &= 0xFF

        # call the state
        if (self.rcvstate == RCV_STATE_IDLE):
            self.RcvStateIdle(data)
        elif(self.rcvstate == RCV_STATE_DSTA):
            self.RcvStateDsta(data)
        elif(self.rcvstate == RCV_STATE_SRCA):
            self.RcvStateSrca(data)
        elif(self.rcvstate == RCV_STATE_CMND):
            self.RcvStateCmnd(data)
        elif(self.rcvstate == RCV_STATE_OPT1):
            self.RcvStateOpt1(data)
        elif(self.rcvstate == RCV_STATE_OPT2):
            self.RcvStateOpt2(data)
        elif(self.rcvstate == RCV_STATE_SEQN):
            self.RcvStateSeqn(data)
        elif(self.rcvstate == RCV_STATE_CTRL):
            self.RcvStateCtrl(data)
        elif(self.rcvstate == RCV_STATE_DATA):
            self.RcvStateData(data)
        elif(self.rcvstate == RCV_STATE_CHEK):
            self.RcvStateChek()
        else:
            self.rcvstate = RCV_STATE_IDLE

        # return the status
        return self.status

    # idle state
    def RcvStateIdle(self, data):
        # check for a start of message
        if (data == CH_DLE):
            self.rcvstate = RCV_STATE_CTRL
            self.buffer.clear()
            self.status = 1

    # destination address
    def RcvStateDsta(self, data):
        # store the destination address
        self.dstaddr = data
        self.rcvstate = RCV_STATE_SRCA

    # source address
    def RcvStateSrca(self, data):
        # store the source address
        self.srcaddr = data
        self.rcvstate = RCV_STATE_CMND

    # command
    def RcvStateCmnd(self, data):
        # store the command/goto option
        self.command = data
        self.rcvstate = RCV_STATE_OPT1

    # option 1
    def RcvStateOpt1(self, data):
        # store the optoin/goto next state
        self.option1 = data
        self.rcvstate = RCV_STATE_OPT2

    # option 2
    def RcvStateOpt2(self, data):
        # store the optoin/goto next state
        self.option2 = data
        if (self.seqenable):
            self.rcvstate = RCV_STATE_SEQN
        else:
            self.rcvstate = RCV_STATE_DATA

    # sequence
    def RcvStateSeqn(self, data):
        # store the sequence number/goto data
        self.rcvsequence = data
        self.rcvstate = RCV_STATE_DATA

    # control
    def RcvStateCtrl(self, data):
        # start of header
        if (data == CH_SOH):
            # reset the checksum
            self.checksum = CH_SOH + CH_DLE

            # check next state
            if (self.multimode):
                self.rcvstate = RCV_STATE_SRCA
            else:
                self.rcvstate = RCV_STATE_CMND

        # start of text
        elif (data == CH_STX):
            self.rcvstate = RCV_STATE_DATA

        # end of transmission
        elif (data == CH_EOT):
            self.rcvstate = RCV_STATE_CHEK

        # special
        elif (data == CH_SPC):
            self.buffer.append(data)
            self.rcvstate = RCV_STATE_DATA

        else:
            # back to idle
            self.rcvstate = RCV_STATE_IDLE

    # data
    def RcvStateData(self, data):
        # check for a DLE
        if (data == CH_DLE):
            self.rcvstate = RCV_STATE_CTRL
        else:
            # stuff data
            self.buffer.append(data)

    def RcvStateChek(self):
        # check for a valid checksum
        if (self.checksum == 0):
            # set message status ok
            self.status = 2
        else:
            self.status = 0

        # set state back idle
        self.rcvstate = RCV_STATE_IDLE
