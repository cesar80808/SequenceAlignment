#include<iostream>
#include<string>
#include<algorithm>

using namespace std;

class sequence_alignment {
    public:

        string s1;
        string s2;
        int s1_len;
        int s2_len;
        int** score_matrix;
        char** arrow_matrix;


        sequence_alignment(string string1, string string2) {

            // Must check beforehand that both have the same length
            s1 = string1;
            s2 = string2;
            
            s1_len = string1.length();
            s2_len = string2.length();

            // score_matrix and arrow_matrix will be of size 
            // (len + 1)x(len + 1)

            score_matrix = new int*[s1_len + 1];
            arrow_matrix = new char*[s1_len + 1];

            for (int i = 0; i <= s1_len; i++) {
                score_matrix[i] = new int[s2_len + 1];
                arrow_matrix[i] = new char[s2_len + 1];
            }

        }

        ~sequence_alignment() {
            for (int i = 0; i <= s1_len; i++) {
                delete[] score_matrix[i];
                delete[] arrow_matrix[i];
            }

            delete[] score_matrix;
            delete[] arrow_matrix;
        }

        void fillMatrix(void) {

            // Fill in the borders
            score_matrix[0][0] = 0;
            arrow_matrix[0][0] = 'E';   // 'E' for "empty"

            for (int j = 1; j <= s2_len; j++) {
                score_matrix[0][j] = -1 * j;
                arrow_matrix[0][j] = 'L';   // 'L' for "left"
            }
            
            for (int i = 1; i <= s1_len; i++) {
                score_matrix[i][0] = -1 * i;
                arrow_matrix[i][0] = 'U';   // 'U' for "up"
            }
            
            // Fill in the middle
            for (int i = 1; i <= s1_len; i++)
                for (int j = 1; j <= s2_len; j++) {
                    int diag_score = (s1[i-1] == s2[j-1] ? 1 : -1);
                    int score = max(score_matrix[i-1][j] - 1, score_matrix[i][j-1] - 1);
                    score = max(score, score_matrix[i-1][j-1] + diag_score);

                    score_matrix[i][j] = score;

                    // Fill arrow_matrix[i][j]

                    // Priority to match/missmatch
                    if (score == score_matrix[i-1][j-1] + diag_score)
                        arrow_matrix[i][j] = 'D';   // 'D' for "diagonal"

                    // Priotity to elimination
                    else if (score == score_matrix[i-1][j] - 1)
                        arrow_matrix[i][j] = 'U';

                    // Priotity to inserion
                    else 
                        arrow_matrix[i][j] = 'L';
                }
        }

        int getMaxScore(void) {
            return score_matrix[s1_len][s2_len];
        }


        void getMaxAlignment(string& s1_res, string& s2_res) {
            s1_res.clear();
            s2_res.clear();

            int i = s1_len;
            int j = s2_len;

            while (i != 0 || j != 0) {

                switch (arrow_matrix[i][j]) {

                    case  'D':
                        s1_res.push_back( s1[i-1] );
                        s2_res.push_back( s2[j-1] );

                        // Move diagonally along the grid
                        i--;
                        j--;
                        
                        break;

                    case  'U':

                        // Insertion in s2
                        s2_res.push_back('-');
                        s1_res.push_back( s1[i-1] );
                        
                        // Move upwards along the grid
                        i--;

                        break;
                    
                    case  'L':

                        // Insertion in s1
                        s1_res.push_back('-');
                        s2_res.push_back( s2[j-1] );
                        
                        // Move leftwards along the grid
                        j--;

                        break;
                
                }   // END SWITCH
            
            }   // END WHILE

            // We reverse the strings, since we were pushing the characters to
            // the back instead of to the front.
            reverse(s1_res.begin(), s1_res.end());
            reverse(s2_res.begin(), s2_res.end());
            
            return;
        }

};

bool validateInput(string str) {
    for (int i = 0; i < str.length(); i++)
        if (str[i] != 'A' && str[i] != 'C' && str[i] != 'G' && str[i] != 'T')
            return false;
    
    return true;
}

int main() {
    string s, t;
    cin >> s >> t;

    if ( !validateInput(s) || !validateInput(t) ) {
        cout << "ERROR";
        return 0;
    }

    sequence_alignment sa1(s, t);
    sa1.fillMatrix();

    string s_res, t_res;
    int res = sa1.getMaxScore();
    sa1.getMaxAlignment(s_res, t_res);

    cout << res << '\n' << s_res << '\n' << t_res;

    return 0;
}