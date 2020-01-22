//
//  main.cpp
//  grou assi3
//
//  Created by Joeun Park and Nabilah Rouf on 10/28/18.
//  Copyright © 2018 Joeun Park and Nabilah Rouf. All rights reserved.
//

#include <iostream>
#include <vector>
#include <stdlib.h>

/*(50 marks) We want to create a program that controls the movements of an object in a 10x10 grid.

Initially, the object is at position (0, 0). The controller supports the following commands:
- up(n): the object moves n cells towards the top of the grid
- down(n): the object moves n cells towards the bottom
- turn right (n): the object turns right and moves n cells
- turn left (n): the object turns right and moves n cells
- reboot: the object comes back to cell (0, 0)

Consider the following grid:
COEN 243 – Fall 2018

9
8
7
6
5
4
3
2
1
0
0 1 2 3 4 5 6 7 8 9

The new grid after executing the sequence: up(5), turn_right(5), down(2) will look like this:
9
8
7
6
5
4
3         o
2
1
0
0 1 2 3 4 5 6 7 8 9
The program takes commands from a user as shown in the following example:
Controller Menu:
1. Up
2. Down
3. Turn Right
4. Turn Left
5. Reboot
6. Show Grid
7. Exit


Command “6. Show Grid” displays the grid. Command “7. Exit” allows exiting the program.
A. Implement the controller’s functionalities. Add necessarily checks to ensure that the object does
not go out of the grid. Your program should be structured using functions. For example, each
command should be implemented in a separate function. (25 marks)
B. We want to add the following commands (25 marks):
- cancel (n): The program cancel the last n operations and return the object to the initial position
- replay (n): The program replays the last n operations that have been cancelled.
- position: The problem displays the position of the object
C. (Bonus) Add a second object to the grid. Repeat questions A and B. The two objects should never
collide, meaning, they should never end up on the same cell (25 marks).
*/

#define NORTH 0// we put names not to be confused with numbers
#define EAST 1
#define SOUTH 2
#define WEST 3

// we're not using double array to save memory space.
// we use X and Y to remember the position of the object.
// If we used a double array, we would ALSO need to remember where the object
// is before we move it. So, we would STILL have to remember its i and j
// position in the double array. So we thought it is not efficient to use double array.

unsigned int n;

using namespace std;

int width = 10;  //index number = 9; starts from (0,0)
int height = 10; //index number = 9; starts from (0,0)


struct Position {
    // We will instantiate many positions because we need to store them in the history
    int x;
    int y;
    int direction;
};

class Controller;

class Controller {
public:
    Controller() {
        sibling = NULL;
        reboot();
    }
    
    void ask_command_and_execute() {
        if (has_exited) { return; }
        
        while(true) {
            // ^ This loop is necessary to deal with cases where the latest
            // command fails.
            
            unsigned int user_command;
            
            unsigned int n = 0;
            
            while (true) {
                cout << "Please input a command from the menu (make sure to provide a number n):" << endl;
                cout << "For example, to go up 3 times, input '1 3' " << endl;
                cout << "And to reboot, input '5 0' " << endl;
                cout << "Command smaller than 1 or bigger than 11 skips. " << endl;
                cout << " 1. Up"  << endl;
                cout << " 2. Down"  << endl;
                cout << " 3. Turn Right"  << endl;
                cout << " 4. Turn Left"  << endl;
                cout << " 5. Reboot"  << endl;
                cout << " 6. Show Grid"  << endl;
                cout << " 7. Exit"  << endl;
                cout << " 8. Cancel " << endl;
                cout << " 9. Replay" << endl;
                cout << " 10. Position" << endl;
                
                // User should input something like:
                // 3 4
                cin >> user_command >> n;
                
                if (user_command > 10 || user_command < 1) {
                    cout << "Please input a correct command number." << endl;
                    continue;
                } else { break ; }
            }
            
            
            bool command_was_successful_and_no_overlap = true;
            
            switch(user_command) {
                case 1:
                    command_was_successful_and_no_overlap = up(n);
                    break;
                case 2:
                    command_was_successful_and_no_overlap = down(n);
                    break;
                case 3:
                    command_was_successful_and_no_overlap = turn_right(n);
                    break;
                case 4:
                    command_was_successful_and_no_overlap = turn_left(n);
                    break;
                case 5:
                    command_was_successful_and_no_overlap = reboot();
                    break;
                case 6:
                    show_grid();
                    break;
                case 7:
                    do_exit();
                    break;
                case 8:
                    command_was_successful_and_no_overlap = cancel(n);
                    break;
                case 9:
                    command_was_successful_and_no_overlap = replay(n);
                    break;
                case 10:
                    position();
                    
                default:
                    // This should never happen because we checked that user_command is between 1 and 7 inclusive.
                    cout << "Wrong input. Please input again, correctly." << endl;
                    break;
            }
            
            if (command_was_successful_and_no_overlap) {
                // Nothing to do.
                return;
            } else {
                cout << "The operation failed. Maybe the new object position"
                << " overlaps with the sibling controller object position."
                << " Try again." << endl;
                continue;
            }
        }
    }
    
    // up(n): the object moves n cells towards the top of the grid
    bool up(unsigned int n) {
        int new_y = y + n;
        cout << "n: " << n << ", oldy: " << y << ", y:" << new_y << endl;
        
        if (new_y>9) { //when the object goes across the border, we put it into the grid.
            new_y = 9;
        }
        
        if (sibling != NULL && sibling->test_if_object_exists_at(x, new_y) ) {
            
            return false;
            
        }
        
        // The (x, new_y) is not overlapping with the sibling (x,y) so we set
        // this y to the new value.
        y = new_y;
        
        save_to_history();
        return true;
    }
    
    
    
    bool down(unsigned int n) {
        int new_y = y-n;
        
        if (new_y<0) { //when the object goes across the border, we put it into the grid.
            new_y=0;
        }
        
        if (sibling != NULL && sibling->test_if_object_exists_at(x, new_y) ) {
            return false;
        }
        
        // The (x, new_y) is not overlapping with the sibling (x,y) so we set
        // this y to the new value.
        y = new_y;
        
        save_to_history();
        return true;
    }
    
    // the object turns right and moves n cells
    bool turn_right(unsigned int n) {
        int old_direction = direction;
        direction = rotate_clockwise(direction);
        
        bool succeded_operation = true;
        
        switch (direction) {
            case NORTH:
                succeded_operation = up(n);
                break;
            case EAST:
                succeded_operation = right(n);
                break;
            case SOUTH:
                succeded_operation = down(n);
                break;
            case WEST:
                succeded_operation = left(n);
                break;
            default:
                return false;
        }
        
        if (!succeded_operation) {
            direction = old_direction;
            return false;
        }
        
        save_to_history();
        return true;
    }
    
    // the object turns left and moves n cells
    bool turn_left (unsigned int n) {
        int old_direction = direction;
        
        direction = rotate_counter_clockwise(direction);
        
        bool succeded_operation = true;
        
        switch (direction) {
            case NORTH:
                succeded_operation = up(n);
                break;
            case EAST:
                succeded_operation = right(n);
                break;
            case SOUTH:
                succeded_operation = down(n);
                break;
            case WEST:
                succeded_operation = left(n);
                break;
            default:
                return false;
                
        }
        
        if (!succeded_operation) {
            direction = old_direction;
            return false;
        }
        
        save_to_history();
        return true;
    }
    
    bool reboot () { //the object comes back to (0,0)
        int new_x = 0;
        int new_y = 0;
        
        if (sibling != NULL && sibling->test_if_object_exists_at(new_x, new_y)) {
            return false;
        }
        
        has_exited = false;
        x=0;
        y=0;
        direction = NORTH; // default
        
        for (int i = 0; i < 100; i++) {
            position_history[i] = NULL;
        }
        
        current_index_in_history = 0;
        
        // Set the history to only have (0,0)
        Position* new_position = (Position*) malloc(sizeof(Position));
        new_position->x = x;
        new_position->y = y;
        new_position->direction = direction;
        position_history[current_index_in_history] = new_position;
        
        return true;
    }
    
    
    void show_grid() { //To display (x,y) on the grid
        for (int i = 9; i >= 0; i--) {
            for (int j = 0; j < 10; j++) {
                
                if (x==j && y==i) {
                    cout<<"[o]"; //print o
                    
                } else {
                    cout<<"[ ]"; //print empty space
                }
                
            }
            
            cout << endl;
        }
        
        // [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
        // [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
        // [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
        // [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
        // [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
        // [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
        // [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
        // [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
        // [ ][ ][ ][ ][ ][ ][ ][o][ ][ ]
        // [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
    }
    
    void do_exit () {
        cout << "You exited." << endl;
        
        has_exited = true;
        
        return;
    }
    
    bool cancel(unsigned int n) {
        int new_current_index_in_history = current_index_in_history - n;
        if (new_current_index_in_history < 0) {
            new_current_index_in_history = 0;
        }
        
        Position* new_position = position_history[new_current_index_in_history];
        
        if (sibling != NULL && sibling->test_if_object_exists_at(new_position->x, new_position->y)) {
            return false;
        }
        
        x = new_position->x;
        y = new_position->y;
        direction = new_position->direction;
        current_index_in_history = new_current_index_in_history;
        return true;
    }
    
    bool replay(unsigned int n) {
        // current_index_in_history = current_index_in_history + n;
        
        int i;
        for (i = 0; i < n; i++) {
            if (position_history[current_index_in_history + i] == NULL) {
                cout << "Cannot go past the last stored position in history";
                // We will not allow the user to go past the last position in
                // the history. Also, we do not want to ask the user to enter a
                // replay(n) again. So, we default to selecting the last
                // position in the history. i is at a position where we have
                // NULL. We'll get the position before that.
                i = i - 1;
                break;
            }
        }
        
        int new_current_index_in_history = current_index_in_history + i;
        
        Position* new_position = position_history[new_current_index_in_history];
        
        if (sibling != NULL && sibling->test_if_object_exists_at(new_position->x, new_position->y)) {
            return false;
        }
        
        current_index_in_history=new_current_index_in_history;
        
        x = new_position->x;
        y = new_position->y;
        direction = new_position->direction;
        
        return true;
    }
    
    void position() {
        cout<<"("<<x<<","<<y<<")"<<endl;
    }
    
    ~Controller() {
        
    }
    
    void think_about_sibling(Controller *sibling_controller) {
        sibling = sibling_controller;
    }
    
    bool test_if_object_exists_at(unsigned int other_x, unsigned int other_y) {
        cout << "x: " << other_x << ", y: "<< other_y << endl;
        return x == other_x && y == other_y;
    }
    
private:
    
    
    void save_to_history() {
        for (int i = current_index_in_history + 1; i < 100; i++) {
            position_history[i] = NULL;
        }
        
        Position* new_position = (Position*) malloc(sizeof(Position));
        new_position->x = x;
        new_position->y = y;
        new_position->direction = direction;
        
        current_index_in_history++;
        position_history[current_index_in_history] = new_position;
    }
    // Example position_history array:
    // [(0,0), (5, 0), (5, 4), (6, 4), (7, 4), (7, 7)]
    //                 ^----- current_index_in_history
    // ^ This situation is if we had made 5 moves and then cancel(4)
    
    
    int rotate_clockwise(unsigned int in_direction) {
        //TO ROTATE RIGHT
        if(in_direction >3) { //NORTH=0, EAST=1, SOUTH =2, WEST =3. If it's bigger than 3 then none of these!
            // direction != NORTH && direction != EAST && direction != SOUTH && direction != WEST
            // error.
            return -1;
        }
        
        switch (direction) {
            case NORTH:
                return EAST;
            case EAST:
                return SOUTH;
            case SOUTH:
                return WEST;
            case WEST:
                return NORTH;
            default:
                return -1;
        }
        
        return -1;
    }
    
    int rotate_counter_clockwise(unsigned int in_direction) {
        //TO ROTATE LEFT
        //COUNTER CLOCKWISE IS EQUAL TO 3 TIMES CLOCKWISE
        return rotate_clockwise(
                                rotate_clockwise(
                                                 rotate_clockwise(
                                                                  in_direction
                                                                  )
                                                 )
                                );
    }
    
    bool right(unsigned int n) {
        int new_x = x+n;
        
        if (new_x>9) { //when the object goes across the border, we put it into the grid.
            new_x = 9;
        }
        
        if (sibling != NULL && sibling->test_if_object_exists_at(new_x, y) ) {
            return false;
        }
        
        x = new_x;
        
        return true;
    }
    
    bool left(unsigned int n) {
        int new_x = x-n;
        if (new_x<0) { //when the object goes across the border, we put it into the grid.
            new_x = 0;
        }
        
        if (sibling != NULL && sibling->test_if_object_exists_at(new_x, y) ) {
            return false;
        }
        
        x = new_x;
        
        return true;
    }
    
    // position history:
    Position* position_history[100];
    int current_index_in_history;
    
    // The current position of the object;
    int x;
    int y;
    int direction;
    
    bool has_exited;
    
    Controller *sibling;
};


int main(int argc, const char * argv[]) {
    Controller my_controller;
    Controller my_second_controller;
    
    my_controller.think_about_sibling(&my_second_controller);
    my_second_controller.think_about_sibling(&my_controller);
    
    while(true) {
        cout << endl;
        cout << "Running first controller: " << endl;
        my_controller.ask_command_and_execute();
        cout << endl;
        cout << "Running second controller: " << endl;
        my_second_controller.ask_command_and_execute();
    }
    
    return 0;
}
