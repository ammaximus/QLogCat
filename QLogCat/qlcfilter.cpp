#include "qlcfilter.h"

bool QLCLine::qt3  = false;

QString QLCLine::getFullMessage()
{
				// TODO use different text codec - old linux qt3.3 koi8-r, new linux qt4.x, windows 1251 qt4.x 
    QTextCodec* codec;
    if (qt3)
        codec = QTextCodec::codecForName("koi8-r");
    else
        codec = QTextCodec::codecForName("utf-8");

    QString fullMessage;
    foreach (QVariant var, messageVector) {
        if (var.canConvert(QVariant::String)){
            if (qt3)
                fullMessage.append(codec->toUnicode(var.toString().toAscii()));
            else
                fullMessage.append(codec->toUnicode(var.toString().toUtf8()));
        }
        else
            fullMessage.append("_?_");
    }
    return fullMessage;
}


QLCFilter::QLCFilter()
    :verbose(true), debug(true), info(true), warning(true), error(true),
      assert(true), fatal(true), textContains(""), logTag("")
{
}

bool QLCFilter::filtrate(QLCLine &line)
{
    if (logTag!="")
        if (line.log!=logTag)
            return false;
    if (textContains!="")
        if(!(line.getFullMessage().contains(&textContains)))
            return false;
    switch(line.level) {
    case QLC::Verbose:
        if (!verbose)
            return false;
        break;
    case QLC::Debug:
        if (!debug)
            return false;
        break;
    case QLC::Info:
        if (!info)
            return false;
        break;
    case QLC::Warning:
        if (!warning)
            return false;
        break;
    case QLC::Error:
        if (!error)
            return false;
        break;
    case QLC::Assert:
        if (!assert)
            return false;
        break;
    case QLC::Fatal:
        if (!fatal)
            return false;
        break;
    }
    return true;
}

bool QLCFilter::setLevelLimit(int level)
{
    switch (level){
    case 0:
        verbose = true;
        debug = true;
        info = true;
        warning = true;
        error = true;
        assert = true;
        fatal = true;
        break;
    case 1:
        verbose = false;
        debug = true;
        info = true;
        warning = true;
        error = true;
        assert = true;
        fatal = true;
        break;
    case 2:
        verbose = false;
        debug = false;
        info = true;
        warning = true;
        error = true;
        assert = true;
        fatal = true;
        break;
    case 3:
        verbose = false;
        debug = false;
        info = false;
        warning = true;
        error = true;
        assert = true;
        fatal = true;
        break;
    case 4:
        verbose = false;
        debug = false;
        info = false;
        warning = false;
        error = true;
        assert = true;
        fatal = true;
        break;
    case 5:
        verbose = false;
        debug = false;
        info = false;
        warning = false;
        error = false;
        assert = true;
        fatal = true;
        break;
    case 6:
        verbose = false;
        debug = false;
        info = false;
        warning = false;
        error = false;
        assert = false;
        fatal = true;
        break;
    }
}
