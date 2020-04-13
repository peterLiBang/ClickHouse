#pragma once
#include <Processors/Merges/IMergingAlgorithmWithSharedChunks.h>
#include <Processors/Merges/MergedData.h>
#include <DataStreams/ColumnGathererStream.h>

namespace Poco
{
class Logger;
}

namespace DB
{

class ReplacingSortedAlgorithm : public IMergingAlgorithmWithSharedChunks
{
public:
    ReplacingSortedAlgorithm(
        const Block & header, size_t num_inputs,
        SortDescription description_, const String & version_column,
        size_t max_block_size,
        WriteBuffer * out_row_sources_buf_ = nullptr,
        bool use_average_block_sizes = false);

    Status merge() override;

private:
    MergedData merged_data;

    ssize_t version_column_number = -1;

    using RowRef = detail::RowRefWithOwnedChunk;
    static constexpr size_t max_row_refs = 2; /// last, current.
    RowRef selected_row; /// Last row with maximum version for current primary key.
    size_t max_pos = 0; /// The position (into current_row_sources) of the row with the highest version.

    /// Sources of rows with the current primary key.
    PODArray<RowSourcePart> current_row_sources;

    void insertRow();
};

}