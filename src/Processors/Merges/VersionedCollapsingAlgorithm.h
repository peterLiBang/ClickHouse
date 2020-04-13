#include <Processors/Merges/IMergingAlgorithmWithSharedChunks.h>
#include <Processors/Merges/MergedData.h>
#include <Processors/Merges/FixedSizeDequeWithGaps.h>
#include <DataStreams/ColumnGathererStream.h>
#include <queue>

namespace DB
{

class VersionedCollapsingAlgorithm : public IMergingAlgorithmWithSharedChunks
{
public:
    /// Don't need version column. It's in primary key.
    VersionedCollapsingAlgorithm(
        const Block & header, size_t num_inputs,
        SortDescription description_, const String & sign_column_,
        size_t max_block_size,
        WriteBuffer * out_row_sources_buf_ = nullptr,
        bool use_average_block_sizes = false);

    Status merge() override;

private:
    MergedData merged_data;

    size_t sign_column_number = 0;

    const size_t max_rows_in_queue;

    /// Rows with the same primary key and sign.
    FixedSizeDequeWithGaps<RowRef> current_keys;
    Int8 sign_in_queue = 0;

    std::queue<RowSourcePart> current_row_sources;   /// Sources of rows with the current primary key

    void insertGap(size_t gap_size);
    void insertRow(size_t skip_rows, const RowRef & row);
};

}