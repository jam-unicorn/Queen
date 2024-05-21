#include "queenDialog.h"

#include <QApplication>

#include <filesystem>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    queenDialog w{std::filesystem::path{argv[0]}.parent_path()};
    return a.exec();
}
