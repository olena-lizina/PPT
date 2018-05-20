#include "MailServiceManager.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
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
    : QObject(parent)
    , host("smtp.gmail.com")
    , port(465)
    , ssl(true)
    , auth(true)
    , user("ppz.donotreply")
    , password("qwerty!@#456")
    , sender("ppz.donotreply@gmail.com")
{

}

MailServiceManager::~MailServiceManager()
{
}

/*static*/ QObject* MailServiceManager::mailServiceManagerProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    MailServiceManager *mailServiceManager = new MailServiceManager();
    return mailServiceManager;
}

/*static*/ void MailServiceManager::setQmlEngine(QQmlApplicationEngine *engine)
{
    MailServiceManager::m_qmlEngine = engine;
}

/*static*/ void MailServiceManager::setSaveManager(std::shared_ptr<SaveManager> saveMgr)
{
    MailServiceManager::mSaveManager = saveMgr;
}

void MailServiceManager::clearComponentCache()
{
    MailServiceManager::m_qmlEngine->clearComponentCache();
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

    zipFile("ppt.db", "ppt.rar");

    message.addPart(new MimeAttachment(new QFile("ppt.rar")));

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
}

void MailServiceManager::zipFile(QString fileName, QString zipName)
{
    QFile::copy(fileName, ("Lectures\\" + fileName));

    QProcess zip;
    zip.start("C:\\Program Files (x86)\\WinRAR\\WinRAR.exe", QStringList() << "a" << zipName << ("Lectures\\" + fileName));

    if (!zip.waitForFinished())
        qDebug() << "WinRAR failed:" << zip.errorString();
    else
        qDebug() << "WinRAR output:" << zip.readAll();

    QFile::remove(("Lectures\\" + fileName));
}
