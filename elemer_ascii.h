#pragma once

#include <QObject>
#include <QVariant>
#include <QVector>

class ElemerASCII {
public:
    //    bool getSuccess(QByteArray data)
    //    {
    //        qDebug() << data;
    //        if (data.isEmpty()) {
    //            return false;
    //        }
    //        int i = 0;
    //        while (data[0] != '!' && data.size()) {
    //            data = data.remove(0, 1);
    //        }
    //        while (data[data.size() - 1] != '\r' && data.size()) {
    //            data = data.remove(data.size() - 1, 1);
    //        }
    //        data = data.remove(data.size() - 1, 1);
    //        QList<QByteArray> list = data.split(';');
    //        data.clear();
    //        while (i < list.count() - 1) {
    //            data.append(list[i++]).append(';');
    //        }
    //        if (calcCrc(data).toInt() == list.last().toInt() && list.count() > 2) {
    //            if (list.at(1) == "$0") {
    //                return true;
    //            }
    //        }
    //        return false;
    //    }

    QByteArray calcCrc(const QByteArray& parcel)
    {
        uint crc = 0xFFFF;
        for (int i = 1; i < parcel.size(); i++) {
            crc ^= parcel.at(i);
            for (int j = 0; j < 8; j++) {
                if (crc & 0x0001) {
                    crc = (crc >> 1) ^ 0xA001;
                } else {
                    crc >>= 1;
                }
            }
        }
        return QByteArray().setNum(crc);
    }

    bool checkParcel(const QByteArray& args, QList<QByteArray>& valyes)
    {
        QByteArray tmp(args);
        int index = 0;
        index = tmp.indexOf('!');
        if (index > 0)
            tmp.remove(0, index);
        index = tmp.lastIndexOf('\r');
        if (index > 0)
            tmp.remove(index, 1);
        index = tmp.lastIndexOf(';') + 1;
        if (index > 0 && calcCrc(tmp.left(index)).toUInt() == tmp.right(tmp.length() - index).toUInt()) {
            tmp.remove(0, 1);
            valyes = tmp.split(';');
            valyes.removeLast();
            return true;
        }
        valyes.clear();
        return false;
    }

    QByteArray createParcel(const QVector<QVariant>& args)
    {
        static QByteArray parcel;
        parcel.clear();
        parcel.append(':');
        for (const QVariant& var : args) {
            bool ok = false;
            switch (var.type()) {
            case QMetaType::Bool: //        1  bool
                parcel.append(QString::number(var.toInt(&ok)));
                break;
            case QMetaType::Int: //         2  int
                parcel.append(QString::number(var.toInt(&ok)));
                break;
            case QMetaType::UInt: //        3  unsigned int
                parcel.append(QString::number(var.toUInt(&ok)));
                break;
            case QMetaType::LongLong: //    4  LongLong
                parcel.append(QString::number(var.toLongLong(&ok)));
                break;
            case QMetaType::ULongLong: //   5  ULongLong
                parcel.append(QString::number(var.toULongLong(&ok)));
                break;
            case QMetaType::Double: //      6  double
                parcel.append(QString::number(var.toDouble(&ok)));
                break;

            case QMetaType::QString: //     10  QString
                parcel.append(var.toString());
                break;
            case QMetaType::QByteArray: //  12  QByteArray
                parcel.append(var.toByteArray());
                break;
            case QMetaType::QDate: //       14  QDate
            case QMetaType::QTime: //       15  QTime
            case QMetaType::QDateTime: //   16  QDateTime
                break;
            case QMetaType::Long: //        32  long
                parcel.append(QString::number(var.toLongLong(&ok)));
                break;
            case QMetaType::Short: //       33  short
                parcel.append(QString::number(var.toInt(&ok)));
                break;
            case QMetaType::Char: //        34  char
                parcel.append(QString::number(var.toInt(&ok)));
                break;
            case QMetaType::ULong: //       35  unsigned long
                parcel.append(QString::number(var.toULongLong(&ok)));
                break;
            case QMetaType::UShort: //      36  unsigned short
                parcel.append(QString::number(var.toUInt(&ok)));
                break;
            case QMetaType::UChar: //       37  unsigned char
                parcel.append(QString::number(var.toUInt(&ok)));
                break;
            case QMetaType::Float: //       38  float
                parcel.append(QString::number(var.toFloat(&ok)));
                break;
            case QMetaType::SChar: //       40  signed char
                parcel.append(QString::number(var.toInt(&ok)));
                break;
            default:
                break;
            }
            parcel.append(';');
        }
        parcel.append(calcCrc(parcel)).append('\r');
        return parcel;
    }
};
