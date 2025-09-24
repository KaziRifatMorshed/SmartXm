#include <filesystem>
#include <iostream>
#include <vector>
#include <string>

class DirectoryScanner {
public:
    // Get all files from the directory (non-recursive)
    static std::vector<std::string> getFiles(const std::string& directoryPath) {
        std::vector<std::string> files;
        
        try {
            if (!std::filesystem::exists(directoryPath)) {
                std::cerr << "Error: Directory does not exist: " << directoryPath << std::endl;
                return files;
            }
            
            if (!std::filesystem::is_directory(directoryPath)) {
                std::cerr << "Error: Path is not a directory: " << directoryPath << std::endl;
                return files;
            }
            
            for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
                if (entry.is_regular_file()) {
                    files.push_back(entry.path().string());
                }
            }
            
        } catch (const std::filesystem::filesystem_error& ex) {
            std::cerr << "Filesystem Error: " << ex.what() << std::endl;
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }
        
        return files;
    }
    
    // Get all files recursively from directory and all subdirectories
    static std::vector<std::string> getFilesRecursive(const std::string& directoryPath) {
        std::vector<std::string> files;
        
        try {
            if (!std::filesystem::exists(directoryPath)) {
                std::cerr << "Error: Directory does not exist: " << directoryPath << std::endl;
                return files;
            }
            
            if (!std::filesystem::is_directory(directoryPath)) {
                std::cerr << "Error: Path is not a directory: " << directoryPath << std::endl;
                return files;
            }
            
            for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath)) {
                if (entry.is_regular_file()) {
                    files.push_back(entry.path().string());
                }
            }
            
        } catch (const std::filesystem::filesystem_error& ex) {
            std::cerr << "Filesystem Error: " << ex.what() << std::endl;
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }
        
        return files;
    }
    
    // Get files with specific extension
    static std::vector<std::string> getFilesByExtension(const std::string& directoryPath, const std::string& extension) {
        std::vector<std::string> files;
        
        try {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath)) {
                if (entry.is_regular_file() && entry.path().extension() == extension) {
                    files.push_back(entry.path().string());
                }
            }
        } catch (const std::filesystem::filesystem_error& ex) {
            std::cerr << "Filesystem Error: " << ex.what() << std::endl;
        }
        
        return files;
    }
    
    // Display files with additional information
    static void displayFilesWithInfo(const std::vector<std::string>& files) {
        std::cout << "\n=== Found " << files.size() << " files ===" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        for (size_t i = 0; i < files.size(); ++i) {
            try {
                std::filesystem::path filePath(files[i]);
                auto fileSize = std::filesystem::file_size(filePath);
                auto lastWrite = std::filesystem::last_write_time(filePath);
                
                std::cout << "[" << (i + 1) << "] " << std::endl;
                std::cout << "   File: " << filePath.filename().string() << std::endl;
                std::cout << "   Path: " << files[i] << std::endl;
                std::cout << "   Size: " << fileSize << " bytes" << std::endl;
                std::cout << "   Extension: " << filePath.extension().string() << std::endl;
                std::cout << std::endl;
                
            } catch (const std::exception& ex) {
                std::cout << "[" << (i + 1) << "] " << files[i] << " (Error getting info: " << ex.what() << ")" << std::endl;
            }
        }
    }
};

int main() {
    // Your specific directory path
    std::string judgeDirectory = R"(D:\Programming for Sessional and Project\SmartXam Project\SmartXm\Tools\Judge)";
    
    std::cout << "Scanning directory: " << judgeDirectory << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    // Option 1: Get files from Judge directory only (not subdirectories)
    std::cout << "\n1. Files in Judge directory only:" << std::endl;
    auto filesInJudge = DirectoryScanner::getFiles(judgeDirectory);
    if (!filesInJudge.empty()) {
        for (const auto& file : filesInJudge) {
            std::cout << "   " << std::filesystem::path(file).filename().string() << std::endl;
        }
    } else {
        std::cout << "   No files found in the directory." << std::endl;
    }
    
    // Option 2: Get all files recursively (including subdirectories)
    std::cout << "\n2. All files recursively (including subdirectories):" << std::endl;
    auto allFiles = DirectoryScanner::getFilesRecursive(judgeDirectory);
    
    if (!allFiles.empty()) {
        DirectoryScanner::displayFilesWithInfo(allFiles);
        
        // Show summary by file types
        std::cout << "\n=== File Type Summary ===" << std::endl;
        std::map<std::string, int> extensionCount;
        for (const auto& file : allFiles) {
            std::string ext = std::filesystem::path(file).extension().string();
            if (ext.empty()) ext = "[no extension]";
            extensionCount[ext]++;
        }
        
        for (const auto& pair : extensionCount) {
            std::cout << "   " << pair.first << ": " << pair.second << " files" << std::endl;
        }
        
    } else {
        std::cout << "   No files found in the directory or its subdirectories." << std::endl;
    }
    
    // Option 3: Get specific file types (example: .cpp, .h, .txt files)
    std::cout << "\n3. Specific file types:" << std::endl;
    std::vector<std::string> extensions = {".cpp", ".h", ".hpp", ".c", ".txt", ".exe", ".dll"};
    
    for (const auto& ext : extensions) {
        auto specificFiles = DirectoryScanner::getFilesByExtension(judgeDirectory, ext);
        if (!specificFiles.empty()) {
            std::cout << "   " << ext << " files (" << specificFiles.size() << "):" << std::endl;
            for (const auto& file : specificFiles) {
                std::cout << "      " << std::filesystem::path(file).filename().string() << std::endl;
            }
        }
    }
    
    // Save file list to a text file
    std::cout << "\n4. Saving file list to 'file_list.txt'..." << std::endl;
    try {
        std::ofstream outFile("file_list.txt");
        outFile << "Files in: " << judgeDirectory << std::endl;
        outFile << "Generated on: " << std::chrono::system_clock::now() << std::endl;
        outFile << std::string(80, '=') << std::endl;
        
        for (size_t i = 0; i < allFiles.size(); ++i) {
            outFile << "[" << (i + 1) << "] " << allFiles[i] << std::endl;
        }
        
        outFile.close();
        std::cout << "   File list saved successfully!" << std::endl;
    } catch (const std::exception& ex) {
        std::cout << "   Error saving file: " << ex.what() << std::endl;
    }
    
    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    
    return 0;
}