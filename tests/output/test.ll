; ModuleID = 'gazprea'
source_filename = "gazprea"

@intFormatStr = global [3 x i8] c"%d\00"
@charFormatStr = global [3 x i8] c"%c\00"
@T = global [2 x i8] c"T\00"
@F = global [2 x i8] c"F\00"
@floatFormatStr = global [3 x i8] c"%g\00"
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

define i32 @fib(i32* %x) {
entry:
  %0 = load i32, i32* %x
  %ieqtmp = icmp eq i32 %0, 1
  br i1 %ieqtmp, label %Then, label %Alt_Then

Then:                                             ; preds = %entry
  ret i32 1

Alt_Then:                                         ; preds = %entry
  br label %merge

merge:                                            ; preds = %Alt_Then
  %1 = load i32, i32* %x
  %ieqtmp1 = icmp eq i32 %1, 0
  br i1 %ieqtmp1, label %Then2, label %Alt_Then2

Then2:                                            ; preds = %merge
  ret i32 0

Alt_Then2:                                        ; preds = %merge
  br label %merge3

merge3:                                           ; preds = %Alt_Then2
  %2 = load i32, i32* %x
  %iaddtmp = sub i32 %2, 1
  %3 = alloca i32
  store i32 %iaddtmp, i32* %3
  %4 = call i32 @fib(i32* %3)
  %5 = alloca i32
  store i32 %4, i32* %5
  %6 = load i32, i32* %x
  %iaddtmp4 = sub i32 %6, 2
  %7 = alloca i32
  store i32 %iaddtmp4, i32* %7
  %8 = call i32 @fib(i32* %7)
  %9 = alloca i32
  store i32 %8, i32* %9
  %10 = load i32, i32* %5
  %11 = load i32, i32* %9
  %iaddtmp5 = add i32 %10, %11
  ret i32 %iaddtmp5
}

define i32 @main() {
entry:
  %0 = alloca i32
  store i32 50, i32* %0
  %1 = call i32 @fib(i32* %0)
  %2 = alloca i32
  store i32 %1, i32* %2
  %3 = load i32, i32* %2
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @intFormatStr, i32 0, i32 0), i32 %3)
  ret i32 0
}

attributes #0 = { nounwind }
