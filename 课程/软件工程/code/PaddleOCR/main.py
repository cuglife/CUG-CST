from paddleocr import PaddleOCR, draw_ocr
from PIL import Image
from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtWidgets import (
    QApplication,
    QFileDialog,
    QWidget,
    QLabel,
    QPushButton,
    QDesktopWidget,
)
from PyQt5.QtGui import QPixmap, QIcon
import sys


class MyPaddle(QWidget):
    def __init__(self):
        super().__init__()
        # img path
        self.img_path = "D:/repos/SoftwareEngineering/myOCR/default.jpg"
        # ocr result
        self.ocr_result = ""
        # ocr accuracy
        self.accuracy = 0
        # window font
        self.font = QtGui.QFont()
        self.font.setFamily("Microsoft YaHei")
        self.font.setPointSize(13)
        self.setWindowTitle("Paddle OCR")
        self.setWindowIcon(QIcon("D:/repos/SoftwareEngineering/myOCR/icon.png"))
        self.resize(1000, 600)
        screen = QDesktopWidget().screenGeometry()
        size = self.geometry()
        self.move(
            (screen.width() - size.width()) / 2, (screen.height() - size.height()) / 2
        )

        # show image
        self.label_image = QLabel("result", self)
        self.image = QPixmap(self.img_path)
        self.label_image.setPixmap(self.image)
        self.label_image.setGeometry(30, 80, 460, 500)
        self.label_image.setScaledContents(True)

        # show accuracy
        self.label_accuracy = QLabel("result accuracy: NaN", self)
        self.label_accuracy.setGeometry(420, 20, 300, 40)
        self.label_accuracy.setFont(self.font)

        # show text result
        self.textEdit = QtWidgets.QTextEdit(self)
        self.textEdit.setGeometry(QtCore.QRect(500, 80, 480, 500))
        self.textEdit.setFont(self.font)
        self.textEdit.setObjectName("textEdit")

        # select button
        button_select_pic = QPushButton("select image", self)
        button_select_pic.setFont(self.font)
        button_select_pic.setGeometry(30, 20, 160, 40)
        button_select_pic.setStyleSheet(
            "QPushButton{color:black}"
            "QPushButton:hover{color:blue}"
            "QPushButton{background-color:rgb(255,255,255)}"
            "QPushButton{border:2px}"
            "QPushButton{border-radius:5px}"
            "QPushButton{padding:2px 4px}"
        )

        button_select_pic.clicked.connect(self.get_path)
        # run button
        button_run = QPushButton("run", self)
        button_run.setFont(self.font)
        button_run.setGeometry(220, 20, 100, 40)
        button_run.setStyleSheet(
            "QPushButton{color:black}"
            "QPushButton:hover{color:blue}"
            "QPushButton{background-color:rgb(255,255,255)}"
            "QPushButton{border:2px}"
            "QPushButton{border-radius:5px}"
            "QPushButton{padding:2px 4px}"
        )
        button_run.clicked.connect(self.run_ocr)
        # show
        self.show()

    def my_ocr(self, img_path):
        # 模型路径下必须含有model和params文件
        # det_model_dir=
        #               '{your_det_model_dir}',
        #               rec_model_dir='{your_rec_model_dir}',
        #               rec_char_dict_path='{your_rec_char_dict_path}',
        #               cls_model_dir='{your_cls_model_dir}', use_angle_cls=True
        ocr = PaddleOCR(use_angle_cls=True, use_gpu=False)

        result = ocr.ocr(img_path, cls=True)
        sum_acc = 0
        for line in result:
            print(line[1])
            self.ocr_result += line[1][0] + "\n"
            sum_acc += line[1][1]

        self.accuracy = sum_acc / len(result)
        print("average accuracy " + str(self.accuracy))
        # 显示结果
        image = Image.open(img_path).convert("RGB")
        boxes = [line[0] for line in result]
        txts = [line[1][0] for line in result]
        scores = [line[1][1] for line in result]
        im_show = draw_ocr(
            image,
            boxes,
            txts,
            scores,
            font_path="C:/Users/null/PaddleOCR/doc/simfang.ttf",
        )
        im_show = Image.fromarray(im_show)
        im_show.save("D:/repos/SoftwareEngineering/myOCR/result.jpg")

    def get_path(self):
        self.img_path, img_type = QFileDialog.getOpenFileName()
        self.image = QPixmap(self.img_path)
        self.label_image.setPixmap(self.image)
        print(self.img_path)

    def run_ocr(self):
        self.ocr_result = ""
        self.my_ocr(self.img_path)
        self.textEdit.setText(self.ocr_result)
        self.label_accuracy.setText("result accuracy: %.7f" % self.accuracy)


# run.window()
if __name__ == "__main__":
    app = QApplication(sys.argv)
    mc = MyPaddle()
    app.exec_()
