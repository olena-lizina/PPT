#include "GradesManager.h"
#include "GradesInfoModel.h"

GradesManager::GradesManager(QObject* parent)
    : ManagerInterface(parent)
{
    mStudents = mSaveManager->loadStudent();
    mReports = mSaveManager->loadReport();
    mLabs = mSaveManager->loadLabWork();
    initModel();
}

/*static*/ QObject* GradesManager::managerProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    GradesManager *manager = new GradesManager();
    return manager;
}

QList<QObject*> GradesManager::studMarks()
{
    return mModel;
}

void GradesManager::initModel()
{
    QList<QObject*> result;

    for (auto rep : mReports)
    {
        auto stud = std::find_if(mStudents.cbegin(), mStudents.cend(),
                                 [&rep](const Student& st){ return rep.studId == st.id; });

        if (mStudents.cend() == stud)
            continue;

        auto lab = std::find_if(mLabs.cbegin(), mLabs.cend(),
                                [&rep](const LabWork& l){ return rep.labId == l.id; });

        if (mLabs.cend() == lab)
            continue;

        result.append(new GradesInfoModel(stud->id, stud->name, lab->id, lab->name, rep.mark));
    }
    mModel = result;
}
