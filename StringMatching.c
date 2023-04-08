#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

//FURKAN DUMAN -2453173

struct mail
{
    int Email_ID;
    char Sender[51];
    char Receipt[51];
    int Day_of_the_month;
    char type[8];
    char* words;
    struct mail* nextMail;

};


void readMails(char* filePath, int number, struct mail* headOfNonSpamMail, struct mail* headOfSpamMail)
{


    char* token;
    char name[50];
    FILE* inFile1;
    char fileName[50];
    char fileNumber[3] = "";
    char type[8];
    int i, x;

    strcat(filePath, "\\");
    for (i = 0; i < number; i++)  // O(N)
    {
        struct mail* C = (struct mail*)malloc(sizeof(struct mail));
        C->nextMail = NULL;

        strcpy(fileName, filePath);
        itoa(i + 1, fileNumber, 10);
        strcat(fileName, fileNumber);
        strcat(fileName, ".txt");


        inFile1 = fopen(fileName, "r");


        if (i == 0)  // If the datapath is written incorrectly, the file will be empty so
            //I can find out that the data path name is misspelled.
        {
            if (inFile1 == NULL)
            {
                printf("There is no datapath !!!\n");
                exit(1);

            }

        }

        if (inFile1 == NULL)  // For more than 50 arguments
            printf("Much argument error!\n");

        fscanf(inFile1, "%d\n", &(C->Email_ID));  // Read integer value and store this value in my structure

        fgets(name, 50, inFile1);
        token = strtok(name, ":");
        token = strtok(NULL, " ");       // I'm reading string values after ':'                                                                      //PROBLEM VAR

        strcpy(C->Sender, token);    // Then store in my structure.

        int size = strlen(C->Sender); //Total size of string
        C->Sender[size - 1] = '\0';  // I am reading strings value with the one space, so here I am deleting the space.

        fgets(name, 50, inFile1);
        token = strtok(name, ":");
        token = strtok(NULL, " ");        // I'm reading string values after ':'

        strcpy(C->Receipt, token);       // Then store in my structure.

        strcpy(C->Receipt, token);  // Then store in my structure.
        size = strlen(C->Receipt); //Total size of string
        C->Receipt[size - 1] = '\0'; // I am reading strings value with the one space, so here I am deleting the space.

        fscanf(inFile1, "%*s %d\n", &C->Day_of_the_month);  //skip string and read integer value, then store in my structure.

        fgets(type, 50, inFile1);
        token = strtok(type, ":");
        token = strtok(NULL, " ");
        strcpy(C->type, token);

        strcpy(C->type, token);  // Then store in my structure.
        size = strlen(C->type); //Total size of string
        C->type[size - 1] = '\0';

        char word[500];
        fgets(word, 500, inFile1);
        int length = strlen(word);

        C->words = (char*)malloc(sizeof(char) * length);
        strcpy(C->words, word);


        //For "Spam" mails
        if ((strcmp(C->type, "Spam") == 0))
        {

            if (headOfSpamMail->nextMail == NULL)
                headOfSpamMail->nextMail = C;

            else
            {
                struct mail* tem_headSpam2 = headOfSpamMail->nextMail;

                while (tem_headSpam2->nextMail != NULL)
                {
                    tem_headSpam2 = tem_headSpam2->nextMail;
                }

                tem_headSpam2->nextMail = C;

            }
        }

        //For "Nonspam" mails
        if (strcmp(C->type, "Nonspam") == 0)
        {

            if (headOfNonSpamMail->nextMail == NULL)
                headOfNonSpamMail->nextMail = C;
            else
            {
                struct mail* tempNonSpam = headOfNonSpamMail->nextMail;
                while (tempNonSpam->nextMail != NULL)
                {
                    tempNonSpam = tempNonSpam->nextMail;
                }

                tempNonSpam->nextMail = C;
            }
        }

    }
}

void searchPattern(char* pattern, struct mail* mails)
{
    int d = 128;
    int q = 256;
    int m = strlen(pattern);
    int h = ((int)(pow(d, (m - 1))) % q);
    int p = 0;
    int t0 = 0;
    int i, s, f, k = 0;
    int wordCounter = 0, mailCounter = 0;

    struct mail* tempMail = mails->nextMail;

    while (tempMail != NULL)
    {
        int whereILeftTheCursor = 0;
        int n = strlen(tempMail->words);
        char* tempWords = (char*)malloc(sizeof(char) * strlen(tempMail->words));


        /*
        	Here I create temporary "words" for each mail. My goal is to convert all letters to lowercase.
         */
        for (i = 0; i < strlen(tempMail->words); i++)
        {

            if (tempMail->words[i] >= 65 && tempMail->words[i] <= 90)
            {
                tempWords[i] = tempMail->words[i] + 32;
            }
            else
            {
                tempWords[i] = tempMail->words[i];
            }

        }
        tempWords[strlen(tempMail->words) - 1] = '\0';// to nullify the end

        for (i = 0; i < m; i++)
        {
            p = ((d * p + pattern[i]) % q);
            t0 = ((d * t0 + tempWords[i]) % q);
        }

        for (s = 0; s <= n - m; s++)
        {
            if (p == t0)
            {
                for (f = 0; f < m; f++)
                {
                    if (pattern[f] != tempWords[s + f])
                    {
                        break;
                    }
                }

                if (f == m)   // If there is a matching , I display correct text.
                {
                    wordCounter++;
                    for (k = whereILeftTheCursor; k < strlen(tempMail->words); k++)
                    {
                        if (k == s)
                        {
                            printf("[");
                            printf("%c", tempMail->words[k]);

                        }
                        else if (k == s + m)
                        {
                            printf("]");
                            printf("%c", tempMail->words[k]);
                            whereILeftTheCursor = k+1;
                            break;
                        }
                        else
                        {
                            printf("%c", tempMail->words[k]);
                        }

                    }
                }
            }

            if (s < n - m)
            {
                t0 = (d * (t0 - (tempWords[s] * h)) + tempWords[s + m]) % q;

                if (t0 < 0)
                    t0 = t0 + q;
            }
        }
        /*I check the cursor where its position lastly.
        */
        if (whereILeftTheCursor != 0)
        {
            for (k = whereILeftTheCursor; k < strlen(tempMail->words); k++)
            {
                printf("%c", tempMail->words[k]);
                if (tempMail->words[k] == '\n')
                    mailCounter++;
            }
        }


        free(tempWords);
        tempMail = tempMail->nextMail;
    }

    printf("%d pattern(s) detected in %d email(s)\n", wordCounter, mailCounter);
}

int main()
{
    int choose, number;

    char c;

    char filePath[50];
    printf("Enter the directory path containing the emails: ");
    scanf("%s", filePath);

    while ((c = getchar()) != '\n' && c != EOF);  // for preventing buffer

    printf("Enter the number of emails to be read: ");
    scanf("%d", &number);
    while ((c = getchar()) != '\n' && c != EOF);

    struct mail* nonSpamList = (struct mail*)malloc(sizeof(struct mail));
    nonSpamList->nextMail = NULL;
    struct mail* SpamList = (struct mail*)malloc(sizeof(struct mail));
    SpamList->nextMail = NULL;


    readMails(filePath, number, nonSpamList, SpamList);

    struct mail* tempSpam = SpamList->nextMail;
    struct mail* tempNon = nonSpamList->nextMail;


    char str[500];
    printf("Enter the pattern: ");
    gets(str);

    /*
    My goal is to convert  letters to lowercase.*/

    int i;
    for (i = 0; i < strlen(str); i++)
    {

        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] = str[i] + 32;
        }
        else
        {
            str[i] = str[i];
        }

    }

    printf("Spam emails containing the pattern:\n");
    searchPattern(str, SpamList);
    printf("Non-spam emails containing the pattern:\n");
    searchPattern(str, nonSpamList);

    return 0;
}
