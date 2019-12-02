const int rows[] = {6, 11, A5, 9, 5, A4, 4, A2};
const int cols[] = {10, 3, 2, 7, A3, 8, 12, 13};
const int maxNum = 8;
int i, j;

void setup() {
  rowCounter = 0;
  colCounter = 0;
}

void loop() {
  for(i = 0; i <= row; i++)
    for(j = 0; j < col; j++) {
      digitalWrite(row[i], LOW);
      digitalWrite(col[i], HIGH);
    }
    //clear
    row = (row+1)%7;
    col = (col+1)%7;
}
