#include "databasehelper.h"

namespace ikoOSKAR {
namespace BLL {

DatabaseHelper::DatabaseHelper()
{
    dal = new ikoOSKAR::DAL::Database();
    databaseCache = dal->GetAllStudents();
}


void DatabaseHelper::Add(Student* s){
    bool result = false;
    try {
        if (this->IdExists(s->id))
            result = dal->Update(s);
        else
            result = dal->Add(s);
        databaseCache->append(*s);
    }  catch (QException e) {

    }
    if (!result){

    }
}

void DatabaseHelper::AddRange(QList<Student *>){

}

void DatabaseHelper::Update(Student* s){

}

void DatabaseHelper::Delete(Student* s){

}

bool DatabaseHelper::IdExists(int id){

}

QList<int> DatabaseHelper::GetAllIds(){

}

void DatabaseHelper::EndOfTheYear(){

}

QList<QString> DatabaseHelper::GetClassNames(){

}

QList<Student*> DatabaseHelper::GetStudentsByClassName(int grade, QString section){

}

Student* DatabaseHelper::GetStudentById(int id){

}

bool DatabaseHelper::CheckForPrerequsities(){
    return false;
}

DatabaseHelper::~DatabaseHelper(){
    delete dal;
    delete databaseCache;
}

} // namespace BLL
} // namespace ikoOSKAR
