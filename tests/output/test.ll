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

declare void @read_in(i8*, i8*, i32)

declare double @pow(double, double)

declare i8* @getEmptyVector(i32)

declare void @initVector(i8*, i32)

declare void @setNullVector(i8*)

declare void @setIdentityVector(i8*)

declare i32 @getVectorLength(i8*)

declare i8* @getReverseVector(i8*)

declare void @printVector(i8*)

declare void @printVectorElement(i8*, i32)

declare void @copyVectorElements(i8*, i8*)

declare i8* @getVectorSlice(i8*, i8*)

declare i8* @getVectorCopy(i8*)

declare i8* @getVectorElementPointer(i8*, i32)

declare i8* @getNewInterval(i32, i32)

declare i8* @getVectorFromInterval(i8*, i32)

declare i8* @getEmptyMatrix()

declare void @initMatrix(i8*, i32, i32)

declare void @setNullMatrix(i8*)

declare void @setIdentityMatrix(i8*)

declare i32 @getNumCols(i8*)

declare i32 @getNumRows(i8*)

declare i8* @indexScalarVector(i32, i8*)

declare i8* @indexVectorScalar(i8*, i32)

declare i8* @indexVectorVector(i8*, i8*)

declare void @printMatrix(i8*)

declare i8* @sliceScalarVector(i32, i8*)

declare i8* @sliceVectorScalar(i8*, i32)

declare i8* @sliceVectorVector(i8*, i8*)

declare i8* @getMatrixElementPointer(i8*, i32, i32)

attributes #0 = { nounwind }
