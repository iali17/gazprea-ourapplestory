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

define i32 @main() {
entry:
  %0 = alloca i32
  store i32 2, i32* %0
  %1 = load i32, i32* %0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @intFormatStr, i32 0, i32 0), i32 %1)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @charFormatStr, i32 0, i32 0), i8 10)
  %4 = load i32, i32* %0
  %iaddtmp = add i32 %4, 1
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @intFormatStr, i32 0, i32 0), i32 %iaddtmp)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @charFormatStr, i32 0, i32 0), i8 10)
  %7 = load i32, i32* %0
  %iaddtmp1 = sub i32 %7, 1
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @intFormatStr, i32 0, i32 0), i32 %iaddtmp1)
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @charFormatStr, i32 0, i32 0), i8 10)
  %10 = load i32, i32* %0
  %imultmp = mul i32 %10, 3
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @intFormatStr, i32 0, i32 0), i32 %imultmp)
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @charFormatStr, i32 0, i32 0), i8 10)
  %13 = load i32, i32* %0
  %upCastIntToReal = sitofp i32 %13 to float
  %fdivtmp = fdiv float %upCastIntToReal, 0x3FB99999A0000000
  %14 = fpext float %fdivtmp to double
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @floatFormatStr, i32 0, i32 0), double %14)
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @charFormatStr, i32 0, i32 0), i8 10)
  %17 = load i32, i32* %0
  %18 = sitofp i32 %17 to double
  %19 = call double @pow(double %18, double 2.000000e+00)
  %20 = fptosi double %19 to i32
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @intFormatStr, i32 0, i32 0), i32 %20)
  %22 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @charFormatStr, i32 0, i32 0), i8 10)
  %23 = load i32, i32* %0
  %iaddtmp2 = sub i32 0, %23
  %24 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @intFormatStr, i32 0, i32 0), i32 %iaddtmp2)
  %25 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @charFormatStr, i32 0, i32 0), i8 10)
  %26 = load i32, i32* %0
  %27 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @intFormatStr, i32 0, i32 0), i32 %26)
  %28 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @charFormatStr, i32 0, i32 0), i8 10)
  ret i32 0
}

attributes #0 = { nounwind }
