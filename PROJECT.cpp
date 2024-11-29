#include<stdio.h>
#include<string.h>
#define maxNotes 50
#define NoteLength 100
#include <stdlib.h>
#include<conio.h>
#define MAX_LINE_LENGTH 80
#include <ctype.h>

 int i;
int notecount=0;int j=1;

struct note {
    char time[20];
    char date[20];
    char text[1000];
}notes[50];
char currentUser[50]; // Stores the current username

// Function to clear the input buffer
void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// General function to validate and get input
void get_valid_input(const char *prompt, char *output, int max_length, int is_int) {
    while (1) {
        printf("%s", prompt);
        if (fgets(output, max_length, stdin) != NULL) {
// Remove newline character from the input
            output[strcspn(output, "\n")] = '\0';
// Check if the input is non-empty
            if (strlen(output) == 0) {
                printf("INVALID INPUT. INPUT SHOULD NOT BE EMPTY.\n");
            } else {
                if (is_int) {
// Check if the input is a valid integer
                    char *endptr;
                    strtol(output, &endptr, 10);
                    if (*endptr != '\0') {
                        printf("INVALID INPUT. PLEASE ENTER A VALID NUMBER.\n");
                    } else {
                        return;
                    }
                } else {
// Check if the input does not contain spaces
                    if (strchr(output, ' ') != NULL) {
                        printf("INVALID INPUT. INPUT SHOULD NOT CONTAIN SPACES.\n");
                    } else {
                        return;
                    }
                }
            }
        } else {
            printf("INVALID INPUT. PLEASE ENTER A VALID STRING.\n");
        }
        clear_input_buffer();
    }
}


// Function to clear notes array and reset note count
void clearNotes() {
    for (int i = 0; i < maxNotes; i++) {
        memset(&notes[i], 0, sizeof(notes[i]));
    }
    notecount = 0;
}

// Function to save notes for the current user
void saveNotesToFile() {
    char filename[60];
    sprintf(filename, "%s_notes.txt", currentUser);
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Unable to save notes.\n");
        return;
    }
    for (int i = 0; i < notecount; i++) {
        fprintf(file, "%s\n%s\n%s\n---\n", notes[i].time, notes[i].date, notes[i].text);
    }
    fclose(file);
}

// Function to load notes for the current user
void loadNotesFromFile() {
    clearNotes(); // Clear any previous notes
    char filename[60];
    sprintf(filename, "%s_notes.txt", currentUser);
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("NO EXISTING NOTES FOR THIS USER.\n");
        return;
    }

    while (fscanf(file, "%19s\n%19s\n%999[^\n]\n---\n", notes[notecount].time, notes[notecount].date, notes[notecount].text) == 3) {
        notecount++;
    }
    fclose(file);
}
//For Validation of Time
int isValidTime(char *time) {
    int hours, minutes;
    if (sscanf(time, "%2d:%2d", &hours, &minutes) != 2 || hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
        return 0;
    }
    return 1;
}
//For Validation of Date
int isValidDate(char *date) {
    int day, month, year;
    if (sscanf(date, "%2d-%2d-%4d", &day, &month, &year) != 3 || day < 1 || day > 31 || month < 1 || month > 12 || year > 2024) {
        return 0;
    }
    return 1;
}
//For Add New Notes
void addNewnote() {
    if (notecount >= maxNotes) {
        printf("CANNOT ADD MORE NOTES\n");
    } else {
        while (1) {
            printf("\n\n\n       ENTER TIME (HH:MM): ");
            scanf("%s", notes[notecount].time);
            if (isValidTime(notes[notecount].time)) break;
            else printf("INVALID TIME FORMAT. PLEASE ENTER AGAIN.\n");
        }
        // Date input with validation
        while (1) {
            printf("\n  ENTER DATE (DD-MM-YYYY): ");
            scanf("%s", notes[notecount].date);
            if (isValidDate(notes[notecount].date)) break;
            else printf("INVALID DATE FORMAT. PLEASE ENTER AGAIN.\n");
        }

        getchar(); // Consume the newline character left by scanf

        printf("  ____________________________________Note:%d_______________________________________\n", j);
        printf(" |______________Time-%s_____________________________Date:%s_____________|\n", notes[notecount].time, notes[notecount].date);
        printf(" |---------------------------------------------------------------------------------|\n");

        char line[100];
        notes[notecount].text[0] = '\0'; 

       while (1) {
    printf(" |"); 
    fgets(line, sizeof(line), stdin);

//     Check for line length exceeding limit 
    if (strlen(line) - 1 > MAX_LINE_LENGTH) { // -1 to exclude the newline character
        
        system("cls"); 
        break; 
    }

    if (strcmp(line, "\n") == 0) {
        printf(" |_______________________________NOTE SAVED________________________________________|\n");
	    break;
    }

    strcat(notes[notecount].text, line);
    printf(" |---------------------------------------------------------------------------------|\n");
  
	
}
if (strlen(line) - 1 <= MAX_LINE_LENGTH) {
	j++;
		 notecount++; // Only increment if the note was saved successfully
		saveNotesToFile();
		 }
		 else{printf("LINE IS TOO LONG. NOTE IS DISCARDED.PLEASE ENTER NEW NOTE...\n");
		 }
      
    }
}
//For View Note Function
void printFormattedNote() {
    if (notecount == 0) {
        printf("No notes to display.\n");
        return;
    }

    int noteIndex;
    printf("Enter the note number to print (1 to %d): ", notecount);
    scanf("%d", &noteIndex);

    if (noteIndex < 1 || noteIndex > notecount) {
        printf("Invalid note number.\n");
        return;
    }

    noteIndex--;

    printf("  ____________________________________Note:%d_______________________________________\n", noteIndex + 1);
    printf(" |______________Time-%s_____________________________Date:%s_____________|\n", notes[noteIndex].time, notes[noteIndex].date);
    printf(" |---------------------------------------------------------------------------------|\n");

    char noteCopy[1000];
    strcpy(noteCopy, notes[noteIndex].text); // Create a copy of the note text to avoid modifying the original
    char *token = strtok(noteCopy, "\n");

    while (token != NULL) {
        printf(" |%-81s|\n", token);
        printf(" |---------------------------------------------------------------------------------|\n");
        token = strtok(NULL, "\n");
    }
    printf(" |_______________________________NOTE SAVED________________________________________|\n\n");
}
//For Editing Note Function
void printNotePage(int noteIndex) {
    printf("  ____________________________________Note:%d_______________________________________\n", noteIndex + 1);
    printf(" |______________Time-%s_____________________________Date:%s_____________|\n", notes[noteIndex].time, notes[noteIndex].date);
    printf(" |---------------------------------------------------------------------------------|\n");

    char noteCopy[1000];
    strcpy(noteCopy, notes[noteIndex].text); // Create a copy of the note text to avoid modifying the original
    char *token = strtok(noteCopy, "\n");

    while (token != NULL) {
        printf(" |%-81s|\n", token);
        printf(" |---------------------------------------------------------------------------------|\n");
        token = strtok(NULL, "\n");
    }
    printf(" |_______________________________NOTE SAVED________________________________________|\n\n");
}

//Edit note Function
void editnote() {
    if (notecount == 0) {
        printf("NO NOTES TO EDIT.\n");
        return;
    }

        int noteIndex=i;
   printf("Enter the note number to edit (1 to %d): ", notecount);
   scanf("%d", &noteIndex);
    getchar();

    if (noteIndex < 1 || noteIndex > notecount) {
        printf("INVALID NOTE NUMBER.\n");
        return;
    }
    noteIndex--; 
    // Print the complete page
    printNotePage(noteIndex);
    while (1) {
        printf("ENTER NEW TIME (HH:MM): ");
        scanf("%s", notes[noteIndex].time);
        if (isValidTime(notes[noteIndex].time)) {
            system("cls");
            printf("TIME UPDATED.\n");
            printNotePage(noteIndex);
            break;
        } else {
            printf("INVALID TIME FORMAT. PLEASE ENTER AGAIN.\n");
        }
    }
    while (1) {
        printf("ENTER NEW DATE (DD-MM-YYYY): ");
        scanf("%s", notes[noteIndex].date);
        if (isValidDate(notes[noteIndex].date)) {
            system("cls");
            printf("DATE UPDATED.\n");
            printNotePage(noteIndex);
            break;
        } else {
            printf("INVALID DATE FORMAT. PLEASE ENTER AGAIN.\n");
        }
    }

    getchar(); // Consume the newline character left by scanf
    char newText[1000];
    newText[0] = '\0'; // Initialize the new text
    printf("ENTER NEW CONTENT. PRESS ENTER TO KEEP CURRENT LINE, OR TYPE NEW CONTENT:\n");

    char line[1000]; 
    char *token = strtok(notes[noteIndex].text, "\n");

    while (token != NULL) {
        printf("CURRENT LINE: %s\n", token);
        printf("ENTER NEW CONTENT FOR THIS LINE (OR PRESS ENTER TO KEEP IT UNCHANGED): ");
        fgets(line, sizeof(line), stdin);

// Remove newline character from the input if present
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) {
            strcat(newText, token); // Keep existing line
        } else {
// Add the new line with automatic wrapping
            int lineLen = strlen(line);
            char tempLine[100];
            int pos = 0;
            while (lineLen > 0) {
                if (lineLen > MAX_LINE_LENGTH) {
                    strncpy(tempLine, line + pos, MAX_LINE_LENGTH);
                    tempLine[MAX_LINE_LENGTH] = '\0';
                    strcat(newText, tempLine);
                    strcat(newText, "\n");
                    pos += MAX_LINE_LENGTH;
                    lineLen -= MAX_LINE_LENGTH;
                } else {
                    strncpy(tempLine, line + pos, lineLen);
                    tempLine[lineLen] = '\0';
                    strcat(newText, tempLine);
                    strcat(newText, "\n");
                    lineLen = 0;
                }
            }
        }

// Print the updated note after each line
        strcpy(notes[noteIndex].text, newText); // Update the note content
        system("cls");
        printf("NOTE UPDATED.\n");
        printNotePage(noteIndex);

        token = strtok(NULL, "\n");
    }

// Option to add new lines
    while (1) {
        printf("DO YOU WANT TO ADD ANOTHER LINE? (YES/NO): ");
        char answer[10];
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = 0;

        if (strcmp(answer, "yes") == 0) {
            printf("ENTER NEW CONTENT: ");
            fgets(line, sizeof(line), stdin);
            line[strcspn(line, "\n")] = 0;

            if (strlen(newText) + strlen(line) < sizeof(newText)) {
                int lineLen = strlen(line);
                char tempLine[100];
                int pos = 0;
                while (lineLen > 0) {
                    if (lineLen > MAX_LINE_LENGTH) {
                        strncpy(tempLine, line + pos, MAX_LINE_LENGTH);
                        tempLine[MAX_LINE_LENGTH] = '\0';
                        strcat(newText, tempLine);
                        strcat(newText, "\n");
                        pos += MAX_LINE_LENGTH;
                        lineLen -= MAX_LINE_LENGTH;
                    } else {
                        strncpy(tempLine, line + pos, lineLen);
                        tempLine[lineLen] = '\0';
                        strcat(newText, tempLine);
                        strcat(newText, "\n");
                        lineLen = 0;
                    }
                }

// Print the updated note after adding a new line
                strcpy(notes[noteIndex].text, newText); // Update the note content
                system("cls");
                printf("NOTE UPDATED.\n");
                printNotePage(noteIndex);
            } else {
                printf("NOTE CONTENT EXCEEDS MAXIMUM ALLOWED LENGTH.\n");
                break;
            }
        } else if (strcmp(answer, "no") == 0) {
            break;
        } else {
            printf("INVALID INPUT. PLEASE ENTER 'YES' OR 'NO'.\n");
        }
    }

// Copy the new text back to the note
    strcpy(notes[noteIndex].text, newText);
    printf("NOTE SAVED.\n");
}
//Delete Note Function
void deletenote() {
    if (notecount == 0) {
        printf("NO NOTES TO DELETE.\n");
        return;
    }
    int noteIndex;
    printf("ENTER THE NOTE NUMBER AGAIN TO DELETE CONFIRMATION (1 to %d): ", notecount);
    scanf("%d", &noteIndex);

    if (noteIndex < 1 || noteIndex > notecount) {
        printf("INVALID NOTE NUMBER.\n");
        return;
    }
    for (int i = noteIndex - 1; i < notecount - 1; i++) {
        strcpy(notes[i].text, notes[i + 1].text);
    }
    notecount--;
    printf("NOTE DELETED SUCCESSFULLY!\n");
    saveNotesToFile();
}
//Exisitng Note Function
void existingnote() {
    if (notecount == 0) {
        printf("\nNO EXISTING NOTES.\n");
        return;
    }
    else{
    printf("\n\n\n*.*.*.*.*.*EXISTING NOTES*.*.*.*.*.*\n\n");
   
      for (i = 0; i < notecount; i++) {
    	printf("%d)_____Time-%s______Note:%d________Date:%s_____\n", i + 1,notes[i].time, i+1, notes[i].date);
    }
    	printf("\n\n         PRESS 1 TO ACCESS NOTE BY TIME\n");
    	printf("\n         PRESS 2 TO ACCESS NOTE BY DATE\n");
    	printf("\n         PRESS 3 TO ACCESS NOTE BY NOTE NUMBER\n");
    	printf("\n         PRESS 4 FOR EXIT\n");
    while(1){
	
char comparetime[50];char comparedate[50];int comparenote;int choices;

			  scanf("        %d",&choices);
        switch (choices){
		case 1 :        printf("ENTER TIME (HH:MM): ");
            scanf("%s", comparetime);
            if (isValidTime(comparetime)) break;
            else printf("INVALID TIME FORMAT. PLEASE ENTER AGAIN.\n"); break;
		case 2 :            printf("ENTER DATE (DD-MM-YYYY): ");
            scanf("%s", comparedate);
            if (isValidDate(comparedate)) break;
            else printf("INVALID DATE FORMAT. PLEASE ENTER AGAIN.\n"); break;
		case 3 :  printf("       ENTER NOTE NUMBER : ");
           scanf("%d", &comparenote);
		   if(comparenote>notecount){printf("Invalid Note Number"); return;
		   } break;
           case 4 :  system("cls"); return;
		default : printf("INVALID CHOICE"); break;
	}for (int i = 0; i < notecount; i++) {if(strcmp(comparedate, notes[i].date) == 0 || strcmp(comparetime, notes[i].time) == 0 || comparenote == (i+1)){
		printf("%d)_____Time-%s______Note:%d________Date:%s_____\n", i + 1,notes[i].time, i + 1, notes[i].date);

while (1) {	int cohice;
                printf("\nMenu\n");
                printf("PRESS 1 FOR EDIT NOTE\n");
                printf("PRESS 2 FOR DELETE NOTE\n");
                printf("PRESS 3 FOR VIEW NOTE\n");
                 printf("PRESS 4 FOR EXIT\n");
                printf("ENTER YOUR CHOICE: ");
                scanf("%d", &cohice);
                system("cls");
                switch (cohice) {
                    case 1: 
                        editnote(); 
                        break;
                    case 2: 
                        deletenote(); 
                        break;
                    case 3:
                    	printFormattedNote();
                    	break;
                    case 4: 
                       return;
                    default: 
                        printf("Invalid Choice\n"); 
                        break;
                }
				}
	
	}
	}
	}
		}
}
// Signup function
void signUp() {
    FILE *usersFile = fopen("users.txt", "a");
    if (!usersFile) {
        printf("ERROR: COULD NOT OPEN USERS FILE.\n");
        return;
    }
    char username[50], password[50];
    // Use get_valid_input to get a valid username
	 get_valid_input("\n\n\n   ENTER USERNAME: ", username, sizeof(username), 0);
	  // Use get_valid_input to get a valid password
	 get_valid_input("\n   ENTER PASSWORD: ", password, sizeof(password), 0);
    fprintf(usersFile, "%s\n%s\n", username, password);
    fclose(usersFile);

    printf("ACCOUNT CREATED SUCCESSFULLY! YOU CAN NOW LOGIN.\n");
}
//Login Function
void login() {
    FILE *usersFile = fopen("users.txt", "r");
    if (!usersFile) {
        printf("NO ACCOUNTS FOUND. PLEASE SIGN UP FIRST.\n");
        return;
    }

    char username[50], password[50];
    char savedUsername[50], savedPassword[50];
    int authenticated = 0;
    printf("\n\n\n\n      ==========");
    printf("\n        Login:-\n");
    printf("      ==========\n\n");
// Input and validation for username
    get_valid_input("         \nENTER USERNAME: ", username, sizeof(username), 0);
// Input and validation for password with masking
    printf("         \nENTER PASSWORD: ");
    int i = 0;
    char ch;
    while (1) {
        ch = getch();  
        if (ch == 13) { 
            password[i] = '\0';  
            break;
        } else if (ch == 8 && i > 0) {  
            i--;
            printf("\b \b");  // Remove the previous '*' character
        } else {
            password[i] = ch;  
            i++;
            printf("*");  
        }
    }
    printf("\n");  
    while (fscanf(usersFile, "%s\n%s\n", savedUsername, savedPassword) != EOF) {
        if (strcmp(savedUsername, username) == 0 && strcmp(savedPassword, password) == 0) {
            strcpy(currentUser, username);
            authenticated = 1;
            break;
        }
    }
    fclose(usersFile);

    if (authenticated==1) {
        system("cls");
        printf("Login successful! Welcome, %s.\n", currentUser);
        loadNotesFromFile();
    } else {
    	 system("cls");
        printf("Invalid username or password.\n");
    }
}


void menu(){
char choice_str[100];
while (1) {
	     
    printf(" ___  __ _____ _   _ _   _         \n");
    printf(" |  \\/  | ____| \\ | | | | |_      \n");
    printf(" | |\\/| |  _| |  \\| | | | (_)____ \n");
    printf(" | |  | | |___| |\\  | |_| ||_____|\n");
    printf(" |_|  |_|_____|_| \\_|\\___/(_)     \n");
                printf("\n  PRESS 1 FOR NEW NOTE\n");
                printf("\n  PRESS 2 FOR EXISTING NOTE\n");
                printf("\n  PRESS 3 FOR EXIT\n");    
        get_valid_input("\n  ENTER YOUR CHOICE: ", choice_str, sizeof(choice_str), 1);
        int choice = atoi(choice_str);
                system("cls");
                switch (choice) {
                    case 1: 
                        addNewnote(); 
                        break;
                    case 2: 
                        existingnote(); 
                        break;
                    case 3: 
                       return;
                    default: 
                        printf("Invalid Choice\n"); 
                        break;
                }
				}
				}
//Main Function
int main() {
	printf("\n\n");
    printf("       _____________________________________________________________\n");
    printf("      / \\ \\   / /_ _| ____\\ \\      / / |  _ \\_ _|  / \\  |  _ \\ \\ / /\n");
    printf("     / _ \\ \\ / / | ||  _|  \\ \\ /\\ / /  | | | | |  / _ \\ | |_) \\ V / \n");
    printf("    / ___ \\ V /  | || |___  \\ V  V /   | |_| | | / ___ \\|  _ < | |  \n");
    printf("   /_/   \\_\\_/  |___|_____|  \\_/\\_/    |____/___/_/   \\_\\_| \\_\\|_|  \n");

	printf("\n\n");
    char choice_str[100];
    while (1) {
    	printf("\n       ***********************\n");
        printf("        WELCOME TO DIARY APP\n");
        printf("       ***********************\n");
        printf("      \n1. SIGN UP\n");
        printf("      \n2. LOGIN\n");
        printf("      \n3. EXIT\n");
        
        get_valid_input("       \nENTER YOUR CHOICE: ", choice_str, sizeof(choice_str), 1);
        int choice = atoi(choice_str);
        system("cls");
        
        switch (choice) {
            case 1: signUp(); break;
            case 2: login(); if (strlen(currentUser) > 0) menu(); break;
            case 3: exit(0);
            default:  system("cls"); printf("INVALID CHOICE.\n");
        }
    }
    return 0;
}
