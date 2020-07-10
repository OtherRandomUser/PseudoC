; ModuleID = 't.c'
source_filename = "t.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store i32 0, i32* %2, align 4
  store i32 %0, i32* %3, align 4
  store i32 2, i32* %4, align 4
  store i32 6, i32* %5, align 4
  ;;; c > 0
  %7 = load i32, i32* %3, align 4
  %8 = icmp sgt i32 %7, 0
  br i1 %8, label %9, label %12

9:                                                ; preds = %1
  ;;; a != 7
  %10 = load i32, i32* %4, align 4
  %11 = icmp ne i32 %10, 7
  br i1 %11, label %15, label %12

12:                                               ; preds = %9, %1
  ;;; b == 9
  %13 = load i32, i32* %5, align 4
  %14 = icmp eq i32 %13, 9
  br i1 %14, label %15, label %17

15:                                               ; preds = %12, %9
  %16 = load i32, i32* %3, align 4
  store i32 %16, i32* %4, align 4
  br label %20

17:                                               ; preds = %12
  %18 = load i32, i32* %5, align 4
  %19 = mul nsw i32 %18, 2
  store i32 %19, i32* %5, align 4
  br label %20




20:                                               ; preds = %17, %15
  %21 = load i32, i32* %3, align 4
  %22 = icmp sgt i32 %21, 0
  br i1 %22, label %23, label %26

23:                                               ; preds = %20
  %24 = load i32, i32* %4, align 4
  %25 = icmp ne i32 %24, 7
  br i1 %25, label %29, label %26

26:                                               ; preds = %23, %20
  %27 = load i32, i32* %5, align 4
  %28 = icmp eq i32 %27, 9
  br label %29



29:                                               ; preds = %26, %23
  %30 = phi i1 [ true, %23 ], [ %28, %26 ]
  %31 = zext i1 %30 to i32
  store i32 %31, i32* %6, align 4
  %32 = load i32, i32* %6, align 4
  ret i32 %32
}

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 10.0.0-4ubuntu1 "}
