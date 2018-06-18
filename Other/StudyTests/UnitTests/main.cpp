#include <QtTest>

#include "CategoryTests.cpp"
#include "SerializationTests.cpp"
#include "TaskTests.cpp"
#include "TestTests.cpp"
#include "EqualsTests.cpp"

int main(int argc, char* argv[])
{
   int status = 0;
   {
      CategoryTests categoryTests;
      status |= QTest::qExec(&categoryTests, argc, argv);
   }

   {
      SerializationTests serializationTests;
      status |= QTest::qExec(&serializationTests, argc, argv);
   }


   {
      TaskTests taskTests;
      status |= QTest::qExec(&taskTests, argc, argv);
   }

   {
      TestTests testTests;
      status |= QTest::qExec(&testTests, argc, argv);
   }

   {
      EqualsTests equalsTests;
      status |= QTest::qExec(&equalsTests, argc, argv);
   }

   return status;
}
