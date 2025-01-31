  #include <vector>
    #include <unordered_map>
    #include <cmath>
    #include <algorithm>
    
    enum CompareOp { EQUAL = 0, GREATER = 1 };
    
    struct CompareExpression {
        int columnIdx;    // Column index (0 for A, 1 for B)
        CompareOp compareOp; // Comparison operator (EQUAL or GREATER)
        int value;        // Value to compare
    };
    
    class CEEngine {
    private:
        struct ColumnStats {
            std::unordered_map<int, int> valueCount; // Frequency count for equality
            int totalCount = 0;                      // Total rows
            int greaterCountThreshold = 100;        // Threshold for maintaining > stats
            std::vector<int> greaterValues;         // Sorted values for efficient > queries
        };
    
        ColumnStats statsA;  // Statistics for column A
        ColumnStats statsB;  // Statistics for column B
        int numRows = 0;     // Total number of rows in the dataset
    
        void updateGreaterValues(ColumnStats &stats) {
            if (stats.totalCount > stats.greaterCountThreshold) {
                stats.greaterValues.clear();
                for (const auto &pair : stats.valueCount) {
                    stats.greaterValues.push_back(pair.first);
                }
                std::sort(stats.greaterValues.begin(), stats.greaterValues.end());
            }
        }
    
    public:
        CEEngine(int num, void *dataExecuter) {
            // Initialization based on num (size of dataset)
            numRows = num;
        }
    
        void insertTuple(const std::vector<int> &tuple) {
            statsA.valueCount[tuple[0]]++;
            statsA.totalCount++;
            statsB.valueCount[tuple[1]]++;
            statsB.totalCount++;
    
            // Update greater values for large datasets
            updateGreaterValues(statsA);
            updateGreaterValues(statsB);
        }
    
        void deleteTuple(const std::vector<int> &tuple, int tupleId) {
            if (statsA.valueCount[tuple[0]] > 0) {
                statsA.valueCount[tuple[0]]--;
                statsA.totalCount--;
            }
            if (statsB.valueCount[tuple[1]] > 0) {
                statsB.valueCount[tuple[1]]--;
                statsB.totalCount--;
            }
    
            // Update greater values for large datasets
            updateGreaterValues(statsA);
            updateGreaterValues(statsB);
        }
    
        int query(const std::vector<CompareExpression> &quals) {
            int result = statsA.totalCount; // Start with total row count
            for (const auto &qual : quals) {
                if (qual.columnIdx == 0) { // Column A
                    if (qual.compareOp == EQUAL) {
                        result = std::min(result, statsA.valueCount[qual.value]);
                    } else if (qual.compareOp == GREATER) {
                        result = std::min(result, (int)(statsA.greaterValues.end() - 
                            std::upper_bound(statsA.greaterValues.begin(), statsA.greaterValues.end(), qual.value)));
                    }
                } else if (qual.columnIdx == 1) { // Column B
                    if (qual.compareOp == EQUAL) {
                        result = std::min(result, statsB.valueCount[qual.value]);
                    } else if (qual.compareOp == GREATER) {
                        result = std::min(result, (int)(statsB.greaterValues.end() - 
                            std::upper_bound(statsB.greaterValues.begin(), statsB.greaterValues.end(), qual.value)));
                    }
                }
            }
            return result;
        }
    
        void prepare() {
            // Any preprocessing steps before operations
            updateGreaterValues(statsA);
            updateGreaterValues(statsB);
        }
    };
    