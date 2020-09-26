// Author : Akide Liu 
// Date : 26/9/20
// License and copyright notice: GNU General Public License v3.0 
// https://www.gnu.org/licenses/gpl-3.0.en.html
// Description : 

//
#include <gtest/gtest.h>
#include <iostream>
#include <algorithm>

#include <dao/include/PeopleDao.h>
#include <database_connection.h>

TEST(PeopleDao_test,listAllUsers) {

    vector<string> check {"student01","tutor01","teacher01","student02","tutor02"};

    vector<People*> result = PeopleDao::listAllUsers();

    for (size_t i = 0; i < check.size(); ++i) {
        ASSERT_EQ(result[i]->getName(),check[i]);
    }

    // cout << result << endl;
     // cout << result << endl;

}



TEST(PeopleDao_test, showAllStudents) {

    vector<string> check {"student01", "student02"};

    vector<People*> result = PeopleDao::showAllStudents();

    for (size_t i = 0; i < check.size(); ++i) {
        ASSERT_EQ(result[i]->getName(),check[i]);
    }

}


TEST(PeopleDao_test, showAllTutors) {

    vector<string> check {"tutor01", "tutor02"};

    vector<People*> result = PeopleDao::showAllTutors();

    for (size_t i = 0; i < check.size(); ++i) {
        ASSERT_EQ(result[i]->getName(),check[i]);
    }

}

TEST(PeopleDao_test, showAllTeachers) {

    vector<string> check {"teacher01"};

    vector<People*> result = PeopleDao::showAllTeachers();

    for (size_t i = 0; i < check.size(); ++i) {
        ASSERT_EQ(result[i]->getName(),check[i]);
    }

}

TEST(PeopleDao_test,selectOnePeopleById) {

    map<int, string> check = {
            {1, "student01"},
            {2,"tutor01"},
            {3,"teacher01"},
    };


    for (const auto& kv : check) {
        People* p = PeopleDao::selectOnePeople(kv.first);

        ASSERT_EQ(p->getName(),kv.second);

        delete p;
    }

    People *p = PeopleDao::selectOnePeople(-1);

    ASSERT_EQ(p, nullptr);

    delete p;
}

TEST(PeopleDao_test,selectPeopleByName) {
    vector<string> check01{"student01", "student02"};
    vector<People *> result01 = PeopleDao::selectPeopleByName("student");

    for (size_t i = 0; i < check01.size(); ++i) {
        ASSERT_EQ(check01[i], result01[i]->getName());
    }

    vector<People *> result02 = PeopleDao::selectPeopleByName("IcanNotBeFound");
    ASSERT_EQ(result02.size(),0);
}

TEST(PeopleDao_test,addNewStudent){

    //user id automatically increased and managed by database

    Student *check01 = new Student(0, "test_student");

    bool result01 =  PeopleDao::addNewStudent(check01);

    EXPECT_EQ(result01, true);

    vector<People *> result02 = PeopleDao::selectPeopleByName(check01->getName());

    EXPECT_EQ(result02.size(),1);
    EXPECT_EQ(result02[0]->getName(), "test_student");
    EXPECT_EQ(result02[0]->getUserLevel(), 1);
    EXPECT_EQ(result02[0]->getPassword(), "-1");
    EXPECT_EQ(result02[0]->getTitle(), "student");
    EXPECT_EQ(result02[0]->isActive1(), true);


    mysql::connection db = database_connection::getConnection();

    db.execute("delete people.* from people where people.name = 'test_student';");

    delete check01;

}

TEST(PeopleDao_test,addNewTutor){

    //user id automatically increased and managed by database

    Tutor *check01 = new Tutor(0, "test_tutor");

    bool result01 =  PeopleDao::addNewTutor(check01);

    EXPECT_EQ(result01, true);

    vector<People *> result02 = PeopleDao::selectPeopleByName(check01->getName());

    EXPECT_EQ(result02.size(),1);
    EXPECT_EQ(result02[0]->getName(), "test_tutor");
    EXPECT_EQ(result02[0]->getUserLevel(), 2);
    EXPECT_EQ(result02[0]->getPassword(), "-1");
    EXPECT_EQ(result02[0]->getTitle(), "tutor");
    EXPECT_EQ(result02[0]->isActive1(), true);


    mysql::connection db = database_connection::getConnection();

    db.execute("delete peopleSubject.* from peopleSubject where peopleSubject.user_id in (select people.user_id from people where people.name = 'test_tutor');");

    db.execute("delete people.* from people where people.name = 'test_tutor';");

    delete check01;

}


TEST(PeopleDao_test, updatePeoplePassword) {
    map<int, string> check{
            {1, "123456"},
            {2, "niashdjk"},
            {3,"kasldb"},
    };


    for (const auto &kv : check) {

       EXPECT_TRUE(PeopleDao::updatePeoplePassword(kv.first, kv.second));

    }

    for (const auto &kv : check) {

        People *p = PeopleDao::selectOnePeople(kv.first);

        EXPECT_EQ(p->getPassword(), kv.second);

        delete p;

        EXPECT_TRUE(PeopleDao::updatePeopleResetPassword(kv.first));

    }

    vector<People *> pCheck = PeopleDao::listAllUsers();

    for (auto & i : pCheck) {
        EXPECT_EQ(i->getPassword(), "-1");
    }


}


TEST(PeopleDao_test, updatePeopleActive){


    for (int i = 0; i < 4; i++) {

        EXPECT_TRUE(PeopleDao::updatePeopleActive(i + 1, false));

        EXPECT_TRUE(PeopleDao::updatePeopleActive(i + 1, true));

    }

    EXPECT_FALSE(PeopleDao::updatePeopleActive(10,false));
    EXPECT_FALSE(PeopleDao::updatePeopleActive(10,true));

}
