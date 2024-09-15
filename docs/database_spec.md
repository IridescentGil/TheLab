# Database Schema

SQLite3 database

## Tables

### exercises

| Column | Data Type | Constraints |
| --- | --- | --- |
| name | TEXT | PRIMARY KEY NOT NULL |
| description | TEXT | |
| muscleGroup | TEXT | |
| musclesTargeted | TEXT | |
| type | TEXT | |

### bodyStats

| Column | Data Type | Constraints |
| --- | --- | --- |
| date | INTEGER | PRIMARY KEY NOT NULL |
| weight | INTEGER | |
| height | INTEGER | |
| age | INTEGER | |
| neckMeasurement | INTEGER | |
| shouldersMeasurement | INTEGER | |
| chestMeasurement | INTEGER | |
| waistMeasurement | INTEGER | |
| hipsMeasurement | INTEGER | |
| upperArmRightMeasurement | INTEGER | |
| upperArmLeft Measurement | INTEGER | |
| foreaarmRightMeasurement | INTEGER | |
| forearmLeftMeasurement | INTEGER | |
| thighRightMeasurement | INTEGER | |
| thighLeftMeasurement | INTEGER | |
| calfRightMeasurement | INTEGER | |
| calfLeftMeasurement | INTEGER | |

### bodyCondition

| Column | Data Type | Constraints |
| --- | --- | --- |
| date | INTEGER | PRIMARY KEY NOT NULL |
| neckCondition | INTEGER | NOT NULL |
| trapeziusCondition | INTEGER | NOT NULL |
| bicepsCondition | INTEGER | NOT NULL |
| tricepsCondition | INTEGER | NOT NULL |
| forearmsCondition | INTEGER | NOT NULL |
| pectorals Condition | INTEGER | NOT NULL |
| absCondition | INTEGER | NOT NULL |
| latsCondition | INTEGER | NOT NULL |
| upperBackCondition | INTEGER | NOT NULL |
| lowerBackCondition | INTEGER | NOT NULL |
| quadsCondition | INTEGER | NOT NULL |
| glutesCondition | INTEGER | NOT NULL |
| hamstringsCondition | INTEGER | NOT NULL |
| calfsCondition | INTEGER | NOT NULL |

### workouts

| Column | Data Type | Constraints |
| --- | --- | --- |
| ID | INTEGER | PRIMARY KEY NOT NULL AUTOINCREMENT |
| WorkoutName | TEXT | NOT NULL |
| exOrderNum | INTEGER | NOT NULL |
| exercise | TEXT | NOT NULL FOREIGN KEY REFERENCES exercises(name) |
| type1 | INTEGER | NOT NULL |
| type2 | INTEGER | |

### workoutPlans

| Column | Data Type | Constraints |
| --- | --- | --- |
| ID | INTEGER | PRIMARY KEY NOT NULL AUTOINCREMENT |
| planName | TEXT | NOT NULL |
| workoutID | INTEGER | NOT NULL FOREIGN KEY REFERENCES workouts(ID) |

### history

| Column | Data Type | Constraints |
| --- | --- | --- |
| ID | INTEGER | PRIMARY KEY NOT NULL AUTOINCREMENT |
| date | INTEGER | NOT NULL |
| exercise | TEXT | NOT NULL FOREIGN KEY REFERENCES exercises(name) |
| workout | TEXT | |
| type1 | TEXT | NOT NULL |
| type2 | TEXT | |

