# imports
from PyQt5 import QtCore, QtSerialPort
from PyQt5.QtWidgets import QComboBox, QDialog, QFormLayout, QLabel, QPushButton, QGroupBox, QDialogButtonBox, QVBoxLayout

# class definition
class CommSettings(QDialog):
    def __init__(self, parent=None):
        super(CommSettings,self).__init__(parent)
        self.createFormGroupBox()

        # create the button box
        buttonBox = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        buttonBox.accepted.connect(self.accept)
        buttonBox.rejected.connect(self.reject)
        
        # create the main layout
        mainLayout = QVBoxLayout()
        mainLayout.addWidget(self.formGroupBox)
        mainLayout.addWidget(buttonBox)
        self.setLayout(mainLayout)
        
        self.setWindowTitle("Communications Settings")
        
        
    # form group box
    def createFormGroupBox(self):
        self.formGroupBox = QGroupBox("Settings")
        self.cboxPort = QComboBox()
        self.cboxBaud = QComboBox()
        layout = QFormLayout()
        layout.addRow(QLabel("Comm Port:"), self.cboxPort)
        layout.addRow(QLabel("Baud Rate:"), self.cboxBaud)
        self.formGroupBox.setLayout(layout)
        
        # fill the port combo box
        self.cboxPort.setInsertPolicy(QComboBox.InsertAlphabetically)
        portsAvail = QtSerialPort.QSerialPortInfo.availablePorts()
        for port in portsAvail:
            self.cboxPort.addItem(port.portName())
        
        # create the baudrate
        # populate the baud rate combox box
        self.cboxBaud.addItems(["300","600","1200","2400","3600","4800","7200","9600","19200","38400","115200","230400"])
        
        
        
