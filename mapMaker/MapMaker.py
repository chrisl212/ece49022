#!/usr/bin/python3

import sys
from PyQt5 import QtCore, QtGui
from PyQt5.QtWidgets import QMainWindow, QApplication, QGraphicsScene, QFileDialog
from PyQt5.QtGui import QPen, QBrush, QColor, QMouseEvent
from PyQt5.QtCore import QLineF, Qt
from MapMakerUi import Ui_MapMakerWindow
import numpy as np
import struct

class MapScene(QGraphicsScene):
    def __init__(self, viewWidth, viewHeight, width=10, height=10):
        super(MapScene, self).__init__()
        self.widthStep = viewWidth / width
        self.heightStep = viewHeight / height
        self.width = width
        self.height = height
        self.path = None

        self.shapes = []
        self.currentShape = []
        self.drawRects()

    def drawLine(self, p1, p2):
        pen = QPen(QColor(255, 0, 0))
        pen.setWidth(1)
        x1 = p1[0]*self.widthStep
        x2 = p2[0]*self.widthStep
        y1 = p1[1]*self.heightStep
        y2 = p2[1]*self.heightStep
        self.addLine(x1, y1, x2, y2, pen=pen)

    def drawRects(self):
        x = 0.0
        y = 0.0
        self.clear()
        pen = QPen(QColor(0, 255, 0))
        pen.setWidth(2)
        for i in range(self.width):
            y = 0.0
            for j in range(self.height):
                self.addRect(x, y, self.widthStep, self.heightStep)
                y += self.heightStep
            x += self.widthStep
        for shape in self.shapes + [self.currentShape]:
            for i, p in enumerate(shape):
                self.addEllipse(p[0]*self.widthStep, p[1]*self.heightStep, 5, 5)
                if i != 0:
                    self.drawLine(shape[i-1], p)

    def mousePressEvent(self, mousePressEvent: QMouseEvent):
        point = mousePressEvent.scenePos()
        x = point.x() / self.widthStep
        y = point.y() / self.heightStep
        self.currentShape.append((x, y))
        self.drawRects()

    def keyPressEvent(self, event):
        if event.key() == Qt.Key_Return:
            self.shapes.append(self.currentShape)
            self.currentShape = []

    def saveToPath(self, filePath):
        with open(filePath, 'wb') as f:
            for shape in self.shapes:
                for i, p in enumerate(shape):
                    x = (int(p[0]*100), int(p[1]*100))
                    paint = (i != 0)
                    f.write(struct.pack('i', x[0]))
                    f.write(struct.pack('i', x[1]))
                    f.write(struct.pack('i', paint))

class MapMaker(QMainWindow, Ui_MapMakerWindow):
    def __init__(self, parent=None):
        super(MapMaker, self).__init__(parent)
        self.setupUi(self)

        self.saveButton.clicked.connect(self.save)
#        self.calcButton.clicked.connect(self.calc)
        self.widthEntry.setText('10')
        self.heightEntry.setText('10')
        self.widthEntry.returnPressed.connect(self.drawGrid)
        self.heightEntry.returnPressed.connect(self.drawGrid)
        self.widthEntry.editingFinished.connect(self.drawGrid)
        self.heightEntry.editingFinished.connect(self.drawGrid)
    
    def save(self):
        fileName = QFileDialog.getSaveFileName(self, 'File')
        if fileName:
            self.gridView.scene().saveToPath(fileName[0])

    def drawGrid(self):
        width = int(self.widthEntry.text())
        height = int(self.heightEntry.text())
        viewWidth = self.gridView.frameGeometry().width()
        viewHeight = self.gridView.frameGeometry().height()
        self.gridView.setScene(MapScene(viewWidth, viewHeight, width, height))

if __name__ == '__main__':
    currentApp = QApplication(sys.argv)
    mapMaker = MapMaker()

    mapMaker.show()
    mapMaker.drawGrid()
    currentApp.exec_()
