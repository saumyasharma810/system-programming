#include<iostream>
#include<filesystem>
#include <unordered_map>
#include <map>
#include <typeinfo>
#include <set>
#include <fstream>
#include <string>
#include <functional>
#include<vector>

namespace fs = std::filesystem;


void directories(std::string file){
    // std::cout <<"aa";
    std::map<std::string,unsigned long long> numberofFiles;
    std::map<std::string,unsigned long long> sizeOfFiles;
    std::map<std::string,unsigned long long> filesInDrives;
    std::map<std::string,unsigned long long> sizeOfDrives;  
    fs::path filePath = file;
    // std::cout << "aa";
    if (fs::exists(filePath)) {
        std::set<fs::path> blacklist;
        while(true) {
            fs::path lastPath;
            try {
                for(auto rdi = fs::recursive_directory_iterator(filePath, fs::directory_options::skip_permission_denied);
                    rdi != fs::recursive_directory_iterator();
                    ++rdi) {
                    auto& de = *rdi;
                    lastPath = de.path();

                    if(blacklist.count(de.path())) {
                        rdi.disable_recursion_pending();
                    }
                    else {
                        if (de.path().has_extension()) {
                            std::string s = de.path().extension().string();
                            numberofFiles[s]++;
                            
                            if (fs::is_regular_file(de)) {
                                sizeOfFiles[s] += fs::file_size(de);
                            }

                            if (de.path().has_root_directory()) {
                                std::string drive = de.path().root_directory().string();
                                filesInDrives[drive]++;

                                if (fs::is_regular_file(de)) {
                                    sizeOfDrives[s] += fs::file_size(de);
                                }
                            }
                        }

                    }
                    ++rdi;
                }
            }
            catch (fs::filesystem_error& fe) {
                if(!blacklist.insert(lastPath).second) {
                    // exception on same path, double error, something went really wrong
                    break;
                }
                else {
                    // we blacklisted a new entry, reset your collected info here,
                    // we need to restart
                    // ...

                    continue;
                }
            }

            // we are through and managed to get all info
            // ...

            break;
        }
        // std::cout << "a " << blacklist.size();
        // for(auto val:blacklist){
        //     std::cout << val <<"\n";
        // }
        std::cout << "FILES ..........\n";
        for(auto val:numberofFiles){
            std::cout << val.first << " have "<<val.second<<" files ";
            if(sizeOfFiles[val.first]!=0) std::cout <<"and size of file is "<< sizeOfFiles[val.first]<<" bytes ";
            std::cout <<"\n";
        }
        std::cout << "DIRECTORIES........\n";
        for(auto val:sizeOfDrives){
            std::cout << val.first <<" directory has " << val.second << " files ";
            if(sizeOfDrives[val.first]!=0) std::cout << "and size = " << sizeOfDrives[val.first] << " bytes ";
            std::cout <<"\n";
        }        
    }else{
        std::cout <<"No file exist\n";
    }
}
void directories(std::string file,std::string extension){
    int size = 0;
    int number = 0;
    // fs::path filePath = file;
    if (fs::exists(file)) {
        std::set<fs::path> blacklist;
        while(true) {
            fs::path lastPath;
            try {
                for(auto rdi = fs::recursive_directory_iterator(file, fs::directory_options::skip_permission_denied);
                    rdi != fs::recursive_directory_iterator();
                    ++rdi) {
                    auto& de = *rdi;
                    lastPath = de.path();

                    if(blacklist.count(de.path())) {
                        rdi.disable_recursion_pending();
                    }
                    else {
                        if (de.path().has_extension()) {
                            std::string s = de.path().extension().string();
                            if(s==extension) number++;
                            // numberofFiles[s]++;
                            
                            if (fs::is_regular_file(de)) {
                                size += fs::file_size(de);
                            }

                            // if (de.path().has_root_directory()) {
                            //     std::string drive = de.path().root_directory().string();
                            //     filesInDrives[drive]++;

                            //     if (fs::is_regular_file(de)) {
                            //         sizeOfDrives[s] += fs::file_size(de);
                            //     }
                            // }
                        }

                    }
                    ++rdi;
                }
            }
            catch (fs::filesystem_error& fe) {
                if(!blacklist.insert(lastPath).second) {
                    // exception on same path, double error, something went really wrong
                    break;
                }
                else {
                    // we blacklisted a new entry, reset your collected info here,
                    // we need to restart
                    // ...

                    continue;
                }
            }

            // we are through and managed to get all info
            // ...

            if(number!=0){
                std::cout << "Number of files are "<<number <<" and total space occupied is equal to ";
                if(size>1e9){
                    std::cout << (double)size/1e9 <<" gB \n";
                }else if(size > 1e6){
                    std::cout << (double)size/1e6 <<" MB \n";
                }else if (size > 1e3){
                    std::cout << (double)size/1e3 <<" KB \n";
                }else{
                    std::cout << (double)size <<" Bytes \n";
                }
            }else{
                std::cout<<"No such file with the given extension\n";
            }

            break;
        }
    }else{
        std::cout <<"No file exist\n";
    }
}

int fileSize(std::string file){
    fs::path filePath = file;
    // Check if the file exists
    int size = 0;
    try
    {
        if (fs::exists(filePath)) {
            // std::cout << "File exists." << std::endl;
            size = fs::file_size(filePath);
            // std::cout << "File size: " << fs::file_size(filePath) << " bytes." << std::endl;
            // std::cout << "Absolute path for " << filePath << " is " << fs::absolute(filePath) << '\n';
        } else {
            std::cout << "File does not exist or Not right path" << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return size;
}

void diskSpace(){
    fs::path filepath = "/Users/saumyasharma/";
    if(fs::exists(filepath)){
        fs::space_info disk = fs::space(filepath);
        std::cout << "Disk path \t\t\tFree Space \t\t\t Utilized Space\n";
        std::cout << fs::absolute(filepath) <<"\t\t"<<(double)disk.free/1e9<<" GB\t\t\t"<<(double)(disk.capacity-disk.free)/1e9<<" GB\n";
    }
    else{
        std::cout << "No file path exists";
    }
}
void LargeFiles(std::string file, int size){
    fs::path filePath = file;
    if (fs::exists(filePath)) {
        std::set<fs::path> blacklist;
        while(true) {
            fs::path lastPath;
            try {
                for(auto rdi = fs::recursive_directory_iterator(filePath, fs::directory_options::skip_permission_denied);
                    rdi != fs::recursive_directory_iterator();
                    ++rdi) {
                    auto& de = *rdi;
                    lastPath = de.path();

                    if(blacklist.count(de.path())) {
                        rdi.disable_recursion_pending();
                    }
                    else {
                        if (fs::is_regular_file(de)) {
                                if(fs::file_size(de) > size*1e6){
                                    std::cout<<"File: "<<lastPath<<"Size in MB: "<<fs::file_size(de)/1e6 <<"\n";
                                }
                        }

                    }
                    ++rdi;
                }
            }
            catch (fs::filesystem_error& fe) {
                if(!blacklist.insert(lastPath).second) {
                    // exception on same path, double error, something went really wrong
                    break;
                }
                else {
                    // we blacklisted a new entry, reset your collected info here,
                    // we need to restart
                    // ...

                    continue;
                }
            }

            // we are through and managed to get all info
            // ...

            break;
        }
    }
    else{
        std::cout <<"No file exist\n";
    }
    
}
std::size_t calculateFileHash(std::string filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        return -1;
    }
    // Read the file content into a string
    std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Calculate the hash of the file content
    std::hash<std::string> hasher;
    return hasher(fileContent);
}
void duplicates(std::string file){
     // Map to store the file hashes and their corresponding paths
    std::unordered_map<std::size_t, fs::path> fileHashes;

    // Iterate over the `std::filesystem::directory_entry` elements
    for (const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(file)) {
        if (fs::is_regular_file(dir_entry)) {
            std::string filePath = dir_entry.path().string();
            std::size_t fileHash = calculateFileHash(dir_entry.path());

            // Check if the hash already exists in the map (duplicate found)
            if (fileHashes.find(fileHash) != fileHashes.end()) {
                std::cout << "Duplicate File Found: " << filePath << " is a duplicate of "
                          << fileHashes[fileHash].string() << std::endl;
            } else {
                // Add the hash and file path to the map
                fileHashes[fileHash] = dir_entry.path();
            }
        }
    }
}
void deleteFile(std::string &file){
    try {
        fs::remove_all(file);
        std::cout << "Deleted directory: " << file << std::endl;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error deleting directory " << file << ": " << e.what() << std::endl;
    }
}
void deleteFilesOfSpecificType(std::string file, std::vector<std::string> extensions) {
    fs::path filePath = file; 
    for (const fs::directory_entry& entry : fs::recursive_directory_iterator(file)) {
        if (fs::is_regular_file(entry)) {
            for (const std::string& ext : extensions) {
                if (entry.path().extension() == ext) {
                    try {
                        fs::remove(entry.path());
                        std::cout << "Deleted: " << entry.path() << std::endl;
                    } catch (const fs::filesystem_error& e) {
                        std::cerr << "Error deleting " << entry.path() << ": " << e.what() << std::endl;
                    }
                    break; // Break if file extension matches any from the list
                }
            }
        }
    }
}
void deleteDuplicateFiles(std::string file){
     // Map to store the file hashes and their corresponding paths
    std::unordered_map<std::size_t, fs::path> fileHashes;

    // Iterate over the `std::filesystem::directory_entry` elements
    for (const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(file)) {
        if (fs::is_regular_file(dir_entry)) {
            std::string filePath = dir_entry.path().string();
            std::size_t fileHash = calculateFileHash(dir_entry.path());

            // Check if the hash already exists in the map (duplicate found)
            if (fileHashes.find(fileHash) != fileHashes.end()) {
                deleteFile(filePath);
            } else {
                // Add the hash and file path to the map
                fileHashes[fileHash] = dir_entry.path();
            }
        }
    }
}

int main(){
    std::string file = "/Users/saumyasharma/Desktop/files";
    std::cout <<"\n";
    while(true){
        std::cout <<"What do you want to do (written with command)\n1)Free Space & Space utilized - fssu\n2)Know the file size - filesz\n3)Give the number & size of files with the specific extension - extfile\n4)Want a list of all files with nuumbers and size - lsfz \n5)Change the file path - chpath\n6)Large Files in there - lf\n7)File Duplicates in the given file - dups\n8)Delete Files - df \n9)If you want to delete all files with given specfic extension - dsf\n10)Delete all duplicate files\n11)End this - end\n";
        std::cout << "Please provide the command\n";
        std::string command1;
        std::cin >> command1;
        if(command1 == "fssu"||command1=="1"){
            diskSpace();
        }else if(command1=="filesz"||command1=="2"){
            int size = fileSize(file);
            if(size>1e9){
                std::cout << (double)size/1e9 <<" gB \n";
            }else if(size > 1e6){
                std::cout << (double)size/1e6 <<" MB \n";
            }else if (size > 1e3){
                std::cout << (double)size/1e3 <<" KB \n";
            }else{
                std::cout << (double)size <<" Bytes \n";
            }
            
        }else if(command1=="extfile"||command1=="3"){
            std::cout << "Please provide the extension ";
            std::string ext;
            std::cin >> ext;
            directories(file,ext); 
        }else if(command1=="lsfz"||command1=="4"){
            directories(file);
        }else if(command1 == "chpath"||command1=="5"){
            std::cout <<"Give the file path(absolute or relative)\n";
            std::cin >> file;
        }
        else if(command1=="lf"||command1=="6"){
            std::cout << "Please provide the size you want to be a large file (in MB)";
            int largefile;
            std::cin >> largefile;
            LargeFiles(file,largefile);
        }else if(command1 == "dups"|| command1=="7"){
            duplicates(file);
        }else if(command1=="df"||command1=="8"){
            while(true)
            {
                std::cout << "Are you sure to delete "<<file <<"\n";
                std::cout << "(YES/NO)"<<"\n";
                std::string st;
                std::cin >> st;
                std::transform(st.begin(), st.end(), st.begin(), ::toupper);
                if(st=="YES"){
                    deleteFile(file);
                    break;
                }
                else if(st=="NO"){
                    break;;
                }
                else{
                    std::cout<<"Invalid Input \n";
                }
            }
        }else if(command1 ==  "dsf" || command1 == "9"){
            std::cout << "Give me the number of types of extension you want to delete\n";
            int x;
            std::cin >> x;
            std::vector<std::string> v(x);
            std::cout << "now please write the extension files" <<"\n";
            for(int i = 0; i< x; i++){
                std::cin >> v[i];
            }
            deleteFilesOfSpecificType(file,v);
        }
        else if(command1 == ""||command1=="10"){
            deleteDuplicateFiles(file);
        }else if(command1 == "end"){
            std::cout <<"Thank You";
            break;
        }
        else{
            std::cout << "no command like this\n";
        }
    }
    
}