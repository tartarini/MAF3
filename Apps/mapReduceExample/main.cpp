#include <QtConcurrentMap>
#include <QStringList>
#include <QCoreApplication>
#include <functional>

namespace
{
  struct UpcaseString : public std::unary_function<const QString&,QString>
  {
    QString operator()(const QString & str) const
      {
        return str.toUpper();
      }
  };

  void concatStrings(QString & result, const QString & str)
  {
    result += str;
  }
}

int main()
{
  QStringList strings;
  strings << "foo" <<  "bar" <<  "baz";

  QFuture<QString> res = QtConcurrent::mappedReduced(strings,UpcaseString(),&concatStrings);
  res.waitForFinished();

  qDebug("%s",qPrintable(res.result()));
}
