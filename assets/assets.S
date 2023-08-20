.section .data.@sym@, "a", %progbits
.type assets_@sym@, %object

.balign 16

.global assets_@sym@
assets_@sym@:
    .incbin "@file@"

.global assets_@sym@_size
    .set assets_@sym@_size, . - assets_@sym@
