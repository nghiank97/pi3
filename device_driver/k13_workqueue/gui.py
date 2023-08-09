#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os, sys, time
import numpy as np
import pyqtgraph as pg
from PyQt5 import QtWidgets,QtGui,QtCore

class GraphicWidget(QtWidgets.QGroupBox):
    def __init__(self, *args, **kwargs):
        super(GraphicWidget, self).__init__()

        self.numberOfSamples = 1000
        self.part_value = None

        pg.setConfigOptions(antialias=True)
        self.plotWidget = pg.PlotWidget()
        self.plotWidget.setTitle(kwargs['name'], **{'color': '#FFFFFF'})
        self.plotWidget.getAxis('left').setTextPen('#FFFFFF')
        self.plotWidget.showGrid(x = True, y = True)
        self.plotWidget.setBackground((0x2E,0x31,0x38))
        self.plotWidget.setStyleSheet("border-radius: 10px;")
        
        box = QtWidgets.QVBoxLayout(self)
        box.addWidget(self.plotWidget)
        self.plotWidget.setYRange(kwargs['min'],kwargs['max'])
        self.plotWidget.getPlotItem().hideAxis('bottom')

        self.plotWidget.addLegend()
        self.timeArray = np.arange(-self.numberOfSamples, 0, 1)

        grad = QtGui.QLinearGradient(0, 0, 0, 3)
        grad.setColorAt(0.1, pg.mkColor('#000000'))
        grad.setColorAt(0.9, pg.mkColor('b'))
        brush = QtGui.QBrush(grad)

        self.signalDataArrays = np.zeros(self.numberOfSamples)
        mpen = pg.mkPen(color=(0x8A,0xB6,0x1B), width=1.5)
        self.signalPlots = pg.PlotDataItem([0],[0],pen=mpen, brush=brush)
        self.plotWidget.addItem(self.signalPlots)
        self.setEnabled(False)

    def upDateGraphic(self, data):
        if data == None:
            return
        self.signalDataArrays = np.roll(self.signalDataArrays, -1)
        self.signalDataArrays[-1] = data
        self.signalPlots.setData(self.timeArray, self.signalDataArrays)
        self.signalPlots.updateItems()

class WidgetWindow(QtWidgets.QGroupBox):
    def __init__(self,*args,**kwargs):
        super(WidgetWindow,self).__init__()

        self.degree_graphics = GraphicWidget(name="DEGREE",min=-1000, max= 1000)
        
        self.load_bt = QtWidgets.QPushButton("LOAD")
        self.load_bt.clicked.connect(self.active_timer)

        self.count_la = QtWidgets.QLabel("0 p")
        self.rotion_la = QtWidgets.QLabel("0 v")
        self.degree_la = QtWidgets.QLabel("0 d")

        box = QtWidgets.QVBoxLayout(self)
        box.addWidget(self.degree_graphics)

        hbox = QtWidgets.QHBoxLayout()
        hbox.addWidget(self.load_bt)
        hbox.addWidget(self.count_la)
        hbox.addWidget(self.rotion_la)
        hbox.addWidget(self.degree_la)

        box.addLayout(hbox)

        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.update_data)
        self.file = os.open("/dev/etx_device", os.O_RDWR)
        
    def active_timer(self):
        if self.load_bt.text() == "LOAD":
            self.load_bt.setText("UPLOAD")
            self.timer.start(10)
        else:
            self.load_bt.setText("LOAD")
            self.timer.stop()

    def s16(self,value):
        return -(value & 0x8000) | (value & 0x7fff)

    def update_data(self):
        rx_date = os.read(self.file,256)
        raw = int(rx_date)
        degree = raw/2400*360

        self.degree_graphics.upDateGraphic(degree)

        self.count_la.setText("{} p".format(raw))
        self.rotion_la.setText("{:.3f} v".format(raw/2400*360))
        self.degree_la.setText("{:.3f} d".format(degree))


if __name__ == "__main__":
    os.system("sudo chmod 777 /dev/etx_device")
    app = QtWidgets.QApplication(sys.argv)
    app.setStyle('Fusion')
    mainwindow = WidgetWindow()
    mainwindow.show()
    sys.exit(app.exec_())
