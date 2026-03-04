.section .vectors, "ax"
.align 4

.global _ResetVector
_ResetVector:
    call0 start_cpu


.global _UserExceptionVector
_UserExceptionVector:
    j exception_handler


.global _KernelExceptionVector
_KernelExceptionVector:
    j exception_handler


.global _Level1Vector
_Level1Vector:
    j interrupt_handler