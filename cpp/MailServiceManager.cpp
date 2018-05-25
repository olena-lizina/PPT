#include "MailServiceManager.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <QProcess>

/*static*/ QQmlApplicationEngine *MailServiceManager::m_qmlEngine = nullptr;
/*static*/ SaveManager::Ptr MailServiceManager::mSaveManager;

const QString educTemplate("Шановний учасник,<br><br>В матеріалах курсу сталися зміни.<br>Щоб отримати їх у своєму застосунку, завантажте вкладений файл, розархівуйте його, та збережіть його до директорії із застосунком.<br><br>З повагою,<br>Команда Розробки ППЗ");
const QString labsTemplate("Шановний учасник,<br><br>Вам була призначена для виконання лабораторна робота на тему \"%1\".<br>Щоб отримати її у своєму застосунку, завантажте вкладений файл, розархівуйте його, та збережіть його до директорії із застосунком.<br><br>З повагою,<br>Команда Розробки ППЗ");
const QString marksTemplate("Шановний учасник,<br><br>Ви отримали оцінку за лабораторну роботу на тему \"%1\".<br>Ваша оцінка:\"%2\".<br>Щоб отримати її у своєму застосунку, завантажте вкладений файл, розархівуйте його, та збережіть його до директорії із застосунком.<br><br>З повагою,<br>Команда Розробки ППЗ");
const QString homeTemplate("Шановний викладач,<br><br>Ви отримали звіт з лабораторної роботи на тему \"%1\".<br>Щоб отримати його у своєму застосунку, завантажте вкладений файл, розархівуйте його, та збережіть його до директорії із застосунком.<br><br>З повагою,<br>Команда Розробки ППЗ");

const QString labsSubj("[ППЗ][\"%1\"]Вам надійшла лабораторна робота");
const QString marksSubj("[ППЗ][\"%1\"]Ви отримали нову оцінку");
const QString homeSubj("[ППЗ][\"%1\"]Звіт з лабораторної роботи");

MailServiceManager::MailServiceManager(QObject* parent)
    : ManagerInterface(parent)
    , host("smtp.gmail.com")
    , port(465)
    , ssl(true)
    , auth(true)
    , user("ppz.donotreply")
    , password("qwerty!@#456")
    , sender("ppz.donotreply@gmail.com")
{

}

/*static*/ QObject* MailServiceManager::managerProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    MailServiceManager *manager = new MailServiceManager();
    return manager;
}

void MailServiceManager::sendEducationMaterials(const QString& courseName, const int& courseId)
{
    SmtpClient smtp (host, port, ssl ? SmtpClient::SslConnection : SmtpClient::TcpConnection);

    MimeMessage message;

    message.setSender(new EmailAddress(sender));

    QString subject("ППЗ \"%1\" Зміни в навчальному курсі");

    message.setSubject(subject.arg(courseName));
    auto emails = mSaveManager->studentsEmails(courseId);

    for (auto email : emails)
    {
        message.addRecipient(new EmailAddress(email));
    }

    MimeHtml content;
    content.setHtml(educTemplate);

    message.addPart(&content);

    zipFile("Lectures", "Lectures.rar");

    message.addPart(new MimeAttachment(new QFile("Lectures.rar")));

    if (!smtp.connectToHost())
    {
        qDebug() << ("Connection Failed");
        return;
    }

    if (auth)
        if (!smtp.login(user, password))
        {
            qDebug() << ("Authentification Failed");
            return;
        }

    if (!smtp.sendMail(message))
    {
        qDebug() << ("Mail sending failed");
        return;
    }
    else
    {
        qDebug() << ("The email was succesfully sent.");
    }

    smtp.quit();

    QFile::remove("Lectures.rar");
}

void MailServiceManager::sendLabWorks(const QString courseName, const int courseId, const QString labName)
{
    qDebug() << "sendLabWorks";

    SmtpClient smtp (host, port, ssl ? SmtpClient::SslConnection : SmtpClient::TcpConnection);

    MimeMessage message;

    message.setSender(new EmailAddress(sender));

    QString subject("ППЗ \"%1\" Вам надійшла лабораторна робота");

    message.setSubject(subject.arg(courseName));
    auto emails = mSaveManager->studentsEmails(courseId);

    for (auto email : emails)
    {
        message.addRecipient(new EmailAddress(email));
    }

    MimeHtml content;
    content.setHtml(labsTemplate.arg(labName));

    message.addPart(&content);

    zipFile("Labs", "Labs.rar");

    message.addPart(new MimeAttachment(new QFile("Labs.rar")));

    if (!smtp.connectToHost())
    {
        qDebug() << ("Connection Failed");
        return;
    }

    if (auth)
        if (!smtp.login(user, password))
        {
            qDebug() << ("Authentification Failed");
            return;
        }

    if (!smtp.sendMail(message))
    {
        qDebug() << ("Mail sending failed");
        return;
    }
    else
    {
        qDebug() << ("The email was succesfully sent.");
    }

    smtp.quit();

    QFile::remove("Labs.rar");
}

void MailServiceManager::zipFile(QString dir, QString zipName)
{
    QString srcFilePath(dir);
    QString tgtFilePath("toSend/"+ dir);

    if (!QDir("toSend").exists())
        QDir().mkdir("toSend");

    copyPath(srcFilePath, tgtFilePath);
    QFile::copy("ppt.db", QString(QString("toSend/") + "ppt.db"));

    QProcess zip;
    zip.start("C:\\Program Files (x86)\\WinRAR\\WinRAR.exe", QStringList() << "a" << zipName << "toSend");

    if (!zip.waitForFinished())
        qDebug() << "WinRAR failed:" << zip.errorString();

    QDir toRm("toSend");
    toRm.removeRecursively();
}

bool MailServiceManager::copyPath(const QString &srcFilePath,
                            const QString &tgtFilePath)
{
    QFileInfo srcFileInfo(srcFilePath);

    if (srcFileInfo.isDir())
    {
        QDir targetDir(tgtFilePath);
        targetDir.cdUp();
        if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
            return false;

        QDir sourceDir(srcFilePath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach (const QString &fileName, fileNames)
        {
            const QString newSrcFilePath
                    = srcFilePath + QLatin1Char('/') + fileName;
            const QString newTgtFilePath
                    = tgtFilePath + QLatin1Char('/') + fileName;
            if (!copyPath(newSrcFilePath, newTgtFilePath))
                return false;
        }
    }
    else
    {
        if (!QFile::copy(srcFilePath, tgtFilePath))
            return false;
    }
    return true;
}
