# Stage 0: Configure the LLVM APT repository (throwaway — not inherited by runtime)
FROM ubuntu:24.04 AS repo-setup

ARG LLVM_VERSION=18

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    gnupg \
    lsb-release \
    wget \
    && rm -rf /var/lib/apt/lists/*

# Import the GPG key into a dedicated keyring and scope it via signed-by=
RUN wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key \
    | gpg --dearmor -o /usr/share/keyrings/llvm.gpg \
    && echo "deb [signed-by=/usr/share/keyrings/llvm.gpg] https://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-${LLVM_VERSION} main" \
       > /etc/apt/sources.list.d/llvm.list

# Stage 1: Build
FROM ubuntu:24.04 AS builder

ARG LLVM_VERSION=18
ARG BUILD_TYPE=Release

ENV DEBIAN_FRONTEND=noninteractive

COPY --from=repo-setup /usr/share/keyrings/llvm.gpg /usr/share/keyrings/llvm.gpg
COPY --from=repo-setup /etc/apt/sources.list.d/llvm.list /etc/apt/sources.list.d/llvm.list

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    clang-${LLVM_VERSION} \
    clang-tidy-${LLVM_VERSION} \
    cmake \
    git \
    libclang-${LLVM_VERSION}-dev \
    llvm-${LLVM_VERSION}-dev \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

RUN cmake \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DClang_DIR=/usr/lib/llvm-${LLVM_VERSION}/lib/cmake/clang \
    -DCMAKE_PROGRAM_PATH=/usr/lib/llvm-${LLVM_VERSION}/bin \
    -S . -B build \
    && cmake --build build --target clang-tidy-gjb8114 -- -j"$(nproc)"

# Stage 2: Runtime image
FROM ubuntu:24.04 AS runtime

ARG LLVM_VERSION=18

ENV DEBIAN_FRONTEND=noninteractive

COPY --from=repo-setup /usr/share/keyrings/llvm.gpg /usr/share/keyrings/llvm.gpg
COPY --from=repo-setup /etc/apt/sources.list.d/llvm.list /etc/apt/sources.list.d/llvm.list

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    clang-tidy-${LLVM_VERSION} \
    && rm -rf /var/lib/apt/lists/*

# Create a convenience symlink so users can call `clang-tidy` directly
RUN ln -sf /usr/bin/clang-tidy-${LLVM_VERSION} /usr/local/bin/clang-tidy

COPY --from=builder /src/build/lib/libclang-tidy-gjb8114.so /usr/local/lib/

WORKDIR /workspace

ENTRYPOINT ["clang-tidy"]
CMD ["--help"]
