#include "app_config.h"

#include <QDir>

AppConfig::AppConfig(QObject *parent) : QObject(parent)
{
  QDir dir(QDir::homePath() + defaultFilePath_);

  if(!dir.exists())
  {
    if(!dir.mkpath("."))
    {
      dir.setPath(QFileInfo(reserveFilePath_).absolutePath());
      if(!dir.mkpath("."))
      {
        badFile_    = true;
        actualPath_ = ActualPath::None;
      }
    }
  }

  QString defaultFileName = QDir::homePath() + defaultFilePath_ + fileName_;
  QString reserveFileName = QFileInfo(reserveFilePath_).absolutePath() + reserveFilePath_ + fileName_;

  if(!file_.exists(defaultFileName))
  {
    file_.setFileName(defaultFileName);
    if(!file_.open(QIODevice::WriteOnly))
    {
      if(!file_.exists(reserveFileName))
      {
        file_.setFileName(reserveFileName);
        if(!file_.open(QIODevice::WriteOnly))
        {
          badFile_ = true;
          actualPath_ = ActualPath::None;
        }
        else
        {
          file_.close();
          actualPath_ = ActualPath::Reserve;
        }
      }
    }
    else
    {
      file_.close();
      actualPath_ = ActualPath::Default;
    }
  }

  switch(actualPath_)
  {
    case ActualPath::Default:
    {
      file_.setFileName(defaultFileName);
      break;
    }
    case ActualPath::Reserve:
    {
      file_.setFileName(reserveFileName);
      break;
    }
    case ActualPath::None:
    {
      badFile_ = true;
      break;
    }
  }
}

void AppConfig::setGeometry(Calculator::AppGeometry geometry)
{
  this->geometry_ = geometry;
  isChangedGerometry_ = true;
}

qint64 AppConfig::readFileGeometry()
{
  if(badFile_)
    throw std::logic_error("Файл не существует или не создан");

  Calculator::AppGeometry geometry;

  if(!file_.open(QIODevice::ReadOnly))
    throw std::logic_error("Файл не был открыт для чтения");

  qint64 readBytes = file_.read(reinterpret_cast<char*>(&geometry), sizeof(Calculator::AppGeometry));
  if(!readBytes)
  {
    file_.close();
    throw std::logic_error("Не удалось прочитать конфигурацию");
  }

  file_.close();
  emit sendAppGeometry(geometry);

  return readBytes;
}

qint64 AppConfig::writeAppGeometry()
{
  if(!isChangedGerometry_)
    return 0;

  if(badFile_)
    throw std::logic_error("Файл не существует или не создан");

  if(!file_.open(QIODevice::WriteOnly | QIODevice::Truncate))
    throw std::logic_error("Файл не был открыт для записи");

  qint64 writeBytes = file_.write(reinterpret_cast<const char*>(&geometry_), sizeof(Calculator::AppGeometry));
  if(!writeBytes)
  {
    file_.close();
    throw std::logic_error("Не удалось записать конфигурацию");
  }

  file_.close();

  return writeBytes;
}
