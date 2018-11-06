; ModuleID = 'gazprea'
source_filename = "gazprea"

%ssss = type { i32, i32 }

@intFormatStr = global [3 x i8] c"%d\00"
@charFormatStr = global [3 x i8] c"%c\00"
@T = global [2 x i8] c"T\00"
@F = global [2 x i8] c"F\00"
@floatFormatStr = global [3 x i8] c"%f\00"
@spaceStr = global [2 x i8] c" \00"
@eolnStr = global [2 x i8] c"\0A\00"
@openSqrBStr = global [2 x i8] c"[\00"
@closeSqrBStr = global [2 x i8] c"]\00"

declare i32 @printf(i8* noalias nocapture, ...)

declare void @free(i8*)

; Function Attrs: nounwind
declare noalias i8* @calloc(i64, i64) #0

declare i32 @scanf(i8*, ...)

declare double @pow(double, double)

define i32 @main() {
entry:
  %0 = alloca i32
  store i32 0, i32* %0
  %1 = alloca i32
  store i32 0, i32* %1
  %2 = alloca %ssss
  ret i32 0
}

attributes #0 = { nounwind }
