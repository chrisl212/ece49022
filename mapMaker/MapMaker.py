#!/usr/bin/python3

import sys
from PyQt5 import QtCore, QtGui
from PyQt5.QtWidgets import QMainWindow, QApplication, QGraphicsScene
from PyQt5.QtGui import QPen, QBrush, QColor, QMouseEvent
from PyQt5.QtCore import QLineF
from MapMakerUi import Ui_MapMakerWindow
import numpy as np
import mlrose

class MapScene(QGraphicsScene):
    def __init__(self, viewWidth, viewHeight, width=10, height=10):
        super(MapScene, self).__init__()
        self.widthStep = viewWidth / width
        self.heightStep = viewHeight / height
        self.width = width
        self.height = height
        self.path = None

        self.rects = np.zeros((self.width, self.height), np.uint8)
        self.drawRects()

    def drawLine(self, p1, p2):
        pen = QPen(QColor(255, 0, 0))
        pen.setWidth(1)
        w = self.widthStep / 2.0
        h = self.heightStep / 2.0
        x1 = p1[0]*self.widthStep + w
        x2 = p2[0]*self.widthStep + w
        y1 = p1[1]*self.heightStep + h
        y2 = p2[1]*self.heightStep + h
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
                val = self.rects[i][j]
                if val == 1:
                    self.addLine(x, y+self.heightStep/2.0, x+self.widthStep, y+self.heightStep/2.0, pen=pen)
                elif val == 2:
                    self.addLine(x+self.widthStep/2.0, y, x+self.widthStep/2.0, y+self.heightStep, pen=pen)
                elif val == 3:
                    self.addLine(x, y, x+self.widthStep, y+self.heightStep, pen=pen)
                elif val == 4:
                    self.addLine(x, y+self.heightStep, x+self.widthStep, y, pen=pen)
                elif val == 5:
                    self.addLine(x, y+self.heightStep/2.0, x+self.widthStep/2.0, y+self.heightStep/2.0, pen=pen)
                    self.addLine(x+self.widthStep/2.0, y+self.heightStep/2.0, x+self.widthStep/2.0, y+self.heightStep, pen=pen)
                elif val == 6:
                    self.addLine(x+self.widthStep/2.0, y+self.heightStep/2.0, x+self.widthStep, y+self.heightStep/2.0, pen=pen)
                    self.addLine(x+self.widthStep/2.0, y+self.heightStep/2.0, x+self.widthStep/2.0, y+self.heightStep, pen=pen)
                elif val == 7:
                    self.addLine(x+self.widthStep/2.0, y+self.heightStep/2.0, x+self.widthStep, y+self.heightStep/2.0, pen=pen)
                    self.addLine(x+self.widthStep/2.0, y, x+self.widthStep/2.0, y+self.heightStep/2.0, pen=pen)
                elif val == 8:
                    self.addLine(x, y+self.heightStep/2.0, x+self.widthStep/2.0, y+self.heightStep/2.0, pen=pen)
                    self.addLine(x+self.widthStep/2.0, y, x+self.widthStep/2.0, y+self.heightStep/2.0, pen=pen)

                self.addRect(x, y, self.widthStep, self.heightStep)
                y += self.heightStep
            x += self.widthStep
        if self.path is not None:
            first = self.path[0]
            prev = first
            for i in range(1, len(self.path)):
                cur = self.path[i]
                self.drawLine(prev, cur)
                prev = self.path[i]
            self.drawLine(prev, first)

    def calc(self):
        coords = np.argwhere(self.rects > 0)
        fitness = mlrose.TravellingSales(coords=coords)
        problem = mlrose.TSPOpt(length=coords.shape[0], fitness_fn=fitness, maximize=False)
        state, length = mlrose.genetic_alg(problem, mutation_prob=0.2, max_attempts=100, random_state=2)
        self.path = coords[state]
        newPath = []
        for i in range(len(self.path)):
            point = self.path[i]
            prev = self.path[i-1]
            if i == len(self.path) - 1:
                nex = self.path[0]
            else:
                nex = self.path[i+1]
            val = self.rects[point[0]][point[1]]
            left = np.array((point[0] - 1, point[1]), dtype=np.int64)
            right = np.array((point[0] + 1, point[1]), dtype=np.int64)
            top = np.array((point[0], point[1] - 1), dtype=np.int64)
            bottom = np.array((point[0], point[1] + 1), dtype=np.int64)
            if val == 1:
                #add left and right point
                if point[0] - prev[0] >= 0:
                    if left[0] >= 0 and not (left[0] == prev[0] and left[1] == prev[1]):
                        newPath.append(left)
                    newPath.append(point)
                    if right[0] < self.width and not (right[0] == nex[0] and right[1] == nex[1]):
                        newPath.append(right)
                else:
                    if right[0] < self.width and not (right[0] == nex[0] and right[1] == nex[1]):
                        newPath.append(right)
                    newPath.append(point)
                    if left[0] >= 0 and not (left[0] == prev[0] and left[1] == prev[1]):
                        newPath.append(left)
            elif val == 2:
                #add top and bottom point
                if point[1] - prev[1] <= 0:
                    if bottom[1] < self.height and not (bottom[0] == nex[0] and bottom[1] == nex[1]):
                        newPath.append(bottom)
                    newPath.append(point)
                    if top[1] >= 0 and not (top[0] == prev[0] and top[1] == prev[1]):
                        newPath.append(top)
                else:
                    if top[1] >= 0 and not (top[0] == prev[0] and top[1] == prev[1]):
                        newPath.append(top)
                    newPath.append(point)
                    if bottom[1] < self.height and not (bottom[0] == nex[0] and bottom[1] == nex[1]):
                        newPath.append(bottom)
            else:
                newPath.append(point)
        self.path = np.array(newPath, dtype=np.int64)
        print(self.path)
        self.drawRects()

    def mousePressEvent(self, mousePressEvent: QMouseEvent):
        point = mousePressEvent.scenePos()
        x = int(point.x() / self.widthStep)
        y = int(point.y() / self.heightStep)
        self.rects[x][y] += 1
        if self.rects[x][y] > 8:
            self.rects[x][y] = 0
        if mousePressEvent.button() == QtCore.Qt.RightButton:
            self.rects[x][y] = 2
        self.drawRects()

class MapMaker(QMainWindow, Ui_MapMakerWindow):
    def __init__(self, parent=None):
        super(MapMaker, self).__init__(parent)
        self.setupUi(self)

        self.saveButton.clicked.connect(self.save)
        self.calcButton.clicked.connect(self.calc)
        self.widthEntry.setText('10')
        self.heightEntry.setText('10')
        self.widthEntry.returnPressed.connect(self.drawGrid)
        self.heightEntry.returnPressed.connect(self.drawGrid)
        self.widthEntry.editingFinished.connect(self.drawGrid)
        self.heightEntry.editingFinished.connect(self.drawGrid)
    
    def save(self):
        print('save')

    def calc(self):
        if self.gridView.scene() is not None:
            self.gridView.scene().calc()

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
