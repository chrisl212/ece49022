# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'MapMaker.ui'
#
# Created by: PyQt5 UI code generator 5.13.0
#
# WARNING! All changes made in this file will be lost!


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_MapMakerWindow(object):
    def setupUi(self, MapMakerWindow):
        MapMakerWindow.setObjectName("MapMakerWindow")
        MapMakerWindow.resize(664, 555)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(MapMakerWindow.sizePolicy().hasHeightForWidth())
        MapMakerWindow.setSizePolicy(sizePolicy)
        self.centralwidget = QtWidgets.QWidget(MapMakerWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.gridLayout = QtWidgets.QGridLayout(self.centralwidget)
        self.gridLayout.setObjectName("gridLayout")
        self.verticalLayout = QtWidgets.QVBoxLayout()
        self.verticalLayout.setObjectName("verticalLayout")
        self.gridView = QtWidgets.QGraphicsView(self.centralwidget)
        self.gridView.setObjectName("gridView")
        self.verticalLayout.addWidget(self.gridView)
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.widthLabel = QtWidgets.QLabel(self.centralwidget)
        self.widthLabel.setObjectName("widthLabel")
        self.horizontalLayout.addWidget(self.widthLabel)
        self.widthEntry = QtWidgets.QLineEdit(self.centralwidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.widthEntry.sizePolicy().hasHeightForWidth())
        self.widthEntry.setSizePolicy(sizePolicy)
        self.widthEntry.setObjectName("widthEntry")
        self.horizontalLayout.addWidget(self.widthEntry)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.heightLabel = QtWidgets.QLabel(self.centralwidget)
        self.heightLabel.setObjectName("heightLabel")
        self.horizontalLayout_2.addWidget(self.heightLabel)
        self.heightEntry = QtWidgets.QLineEdit(self.centralwidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.heightEntry.sizePolicy().hasHeightForWidth())
        self.heightEntry.setSizePolicy(sizePolicy)
        self.heightEntry.setObjectName("heightEntry")
        self.horizontalLayout_2.addWidget(self.heightEntry)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        self.horizontalLayout_3 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.calcButton = QtWidgets.QPushButton(self.centralwidget)
        self.calcButton.setObjectName("calcButton")
        self.horizontalLayout_3.addWidget(self.calcButton)
        self.saveButton = QtWidgets.QPushButton(self.centralwidget)
        self.saveButton.setObjectName("saveButton")
        self.horizontalLayout_3.addWidget(self.saveButton)
        self.verticalLayout.addLayout(self.horizontalLayout_3)
        self.gridLayout.addLayout(self.verticalLayout, 0, 0, 1, 1)
        MapMakerWindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(MapMakerWindow)
        QtCore.QMetaObject.connectSlotsByName(MapMakerWindow)

    def retranslateUi(self, MapMakerWindow):
        _translate = QtCore.QCoreApplication.translate
        MapMakerWindow.setWindowTitle(_translate("MapMakerWindow", "Map Maker"))
        self.widthLabel.setText(_translate("MapMakerWindow", "Width:"))
        self.heightLabel.setText(_translate("MapMakerWindow", "Height:"))
        self.calcButton.setText(_translate("MapMakerWindow", "Calculate"))
        self.saveButton.setText(_translate("MapMakerWindow", "Save"))
